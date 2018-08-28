/*
 * Copyright (C) 2017 XRADIO TECHNOLOGY CO., LTD. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the
 *       distribution.
 *    3. Neither the name of XRADIO TECHNOLOGY CO., LTD. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../hal_base.h"

#include "driver/chip/sdmmc/hal_sdhost.h"
#include "driver/chip/sdmmc/sdmmc.h"
#ifdef CONFIG_USE_SDIO
#include "driver/chip/sdmmc/sdio.h"
#endif
#include "sdhost.h"
#include "core.h"
#ifdef CONFIG_USE_SDIO
#include "_sdio.h"
#endif
#ifdef CONFIG_USE_SD
#include "_sd.h"
#endif
#ifdef CONFIG_USE_MMC
#include "_mmc.h"
#endif

/**
 *	mmc_wait_for_req - start a request and wait for completion
 *	@host: MMC host to start command
 *	@mrq: MMC request to start
 *
 *	Start a new MMC custom command request for a host, and wait
 *	for the command to complete. Does not attempt to parse the
 *	response.
 */
__xip_text
int32_t mmc_wait_for_req(struct mmc_host *host, struct mmc_request *mrq)
{
	mrq->cmd->data = mrq->data;
	return HAL_SDC_Request(host, mrq);
}

/**
 *	mmc_wait_for_cmd - start a command and wait for completion
 *	@host: MMC host to start command
 *	@cmd: MMC command to start
 *	@retries: maximum number of retries
 *
 *	Start a new MMC command for a host, and wait for the command
 *	to complete.  Return any error that occurred while the command
 *	was executing.  Do not attempt to parse the response.
 */
__xip_text
int32_t mmc_wait_for_cmd(struct mmc_host *host, struct mmc_command *cmd)
{
	struct mmc_request mrq = {0};

	memset(cmd->resp, 0, sizeof(cmd->resp));

	mrq.cmd = cmd;

	return mmc_wait_for_req(host, &mrq);
}

/**
 *	mmc_align_data_size - pads a transfer size to a more optimal value
 *	@card: the MMC card associated with the data transfer
 *	@sz: original transfer size
 *
 *	Pads the original data size with a number of extra bytes in
 *	order to avoid controller bugs and/or performance hits
 *	(e.g. some controllers revert to PIO for certain sizes).
 *
 *	Returns the improved size, which might be unmodified.
 *
 *	Note that this function is only relevant when issuing a
 *	single scatter gather entry.
 */
__xip_text
int32_t mmc_align_data_size(struct mmc_card *card, uint32_t sz)
{
	/*
	 * FIXME: We don't have a system for the controller to tell
	 * the core about its problems yet, so for now we just 32-bit
	 * align the size.
	 */
	sz = ((sz + 3) / 4) * 4;

	return sz;
}

static inline void mmc_host_clk_hold(struct mmc_host *host)
{
	HAL_SDC_Clk_PWR_Opt(host, 1, 0);
}

static inline void mmc_host_clk_release(struct mmc_host *host)
{
	HAL_SDC_Clk_PWR_Opt(host, 0, 0);
}

/*
 * Apply power to the MMC stack.  This is a two-stage process.
 * First, we enable power to the card without the clock running.
 * We then wait a bit for the power to stabilise.  Finally,
 * enable the bus drivers and clock to the card.
 *
 * We must _NOT_ enable the clock prior to power stablising.
 *
 * If a host does all the power sequencing itself, ignore the
 * initial MMC_POWER_UP stage.
 */
__xip_text
static void mmc_power_up(struct mmc_host *host)
{
	//int bit;

	mmc_host_clk_hold(host);

	/* If ocr is set, we use it */
	//if (host->ocr)
	//	bit = ffs(host->ocr) - 1;
	//else
	//	bit = fls(host->ocr_avail) - 1;

	HAL_SDC_PowerOn(host);

	/*
	 * This delay must be at least 74 clock sizes, or 1 ms, or the
	 * time required to reach a stable voltage.
	 */
	mmc_mdelay(10);

	mmc_host_clk_release(host);
}

__xip_text
static void mmc_power_off(struct mmc_host *host)
{
	mmc_host_clk_hold(host);

	/*
	 * For eMMC 4.5 device send AWAKE command before
	 * POWER_OFF_NOTIFY command, because in sleep state
	 * eMMC 4.5 devices respond to only RESET and AWAKE cmd
	 */
	//if (host->card && mmc_card_is_sleep(host->card)) {
	//	mmc_poweroff_notify(host);
	//}

	/*
	 * Reset ocr mask to be the highest possible voltage supported for
	 * this mmc host. This value will be used at next power up.
	 */
	//host->ocr = 1 << (fls(host->ocr_avail) - 1);

	HAL_SDC_PowerOff(host);
	/*
	 * Some configurations, such as the 802.11 SDIO card in the OLPC
	 * XO-1.5, require a short delay after poweroff before the card
	 * can be successfully turned on again.
	 */
	mmc_mdelay(1);

	mmc_host_clk_release(host);
}

__xip_text
static int32_t mmc_go_idle(struct mmc_host *host)
{
	int32_t err;
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_GO_IDLE_STATE;
	cmd.arg = 0;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_NONE | MMC_CMD_BC;

	err = mmc_wait_for_cmd(host, &cmd);

	mmc_mdelay(1);

	return err;
}

__xip_text
int32_t mmc_send_status(struct mmc_card *card, uint32_t *status)
{
	int32_t err;
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_SEND_STATUS;
	cmd.arg = card->rca << 16;
	cmd.flags = MMC_RSP_SPI_R2 | MMC_RSP_R1 | MMC_CMD_AC;

	err = mmc_wait_for_cmd(card->host, &cmd);
	if (err)
		return err;

	/* NOTE: callers are required to understand the difference
	 * between "native" and SPI format status words!
	 */
	if (status)
		*status = cmd.resp[0];

	return 0;
}

#if ((defined CONFIG_USE_SD) || (defined CONFIG_USE_MMC))
__xip_text
int32_t mmc_sd_switch(struct mmc_card *card, uint8_t mode, uint8_t group,
                             uint16_t value, uint8_t *resp)
{
	struct mmc_request mrq;
	struct mmc_command cmd = {0};
	struct mmc_data data = {0};
	struct scatterlist sg;

	/* NOTE: caller guarantees resp is heap-allocated */

	mode = !!mode;
	value &= 0xF;

	mrq.cmd = &cmd;
	mrq.data = &data;

	cmd.opcode = SD_SWITCH;
	cmd.arg = mode << 31 | 0x00FFFFFF;
	cmd.arg &= ~(0xF << (group * 4));
	cmd.arg |= value << (group * 4);
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;

	data.blksz = 64;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;
	data.sg = &sg;
	data.sg_len = 1;

	sg.len = 64;
	sg.buffer = resp;

	if (mmc_wait_for_req(card->host, &mrq)) {
		return -1;
	}

	return 0;
}

__xip_text
static int32_t mmc_switch(struct mmc_card *card, uint8_t set, uint8_t index, uint8_t value)
{
	struct mmc_command cmd = {0};
	int32_t ret;
	uint32_t status = 0;

	cmd.opcode = MMC_SWITCH;
	cmd.arg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) | (index << 16) | (value << 8) | set;
	cmd.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;

	if (mmc_wait_for_cmd(card->host, &cmd)) {
		return -1;
	}

	/* Must check status to be sure of no errors */
	do {
		ret = mmc_send_status(card, &status);
		if (ret)
			return ret;
	} while (R1_CURRENT_STATE(status) == R1_STATE_PRG);

	if (status & 0xFDFFA000)
		SD_LOGW("unexpected status %x after switch", status);
	if (status & R1_SWITCH_ERROR)
		return -1;

	return 0;
}

__xip_text
int32_t mmc_switch_part(struct mmc_card *card, uint32_t part_num)
{
	return mmc_switch(card, MMC_EXT_CSD_CMD_SET_NORMAL, MMC_EXT_CSD_PART_CONF,
			  (card->extcsd.part_config & ~MMC_SWITCH_PART_ACCESS_MASK)
			  | (part_num & MMC_SWITCH_PART_ACCESS_MASK));
}

__xip_text
int32_t mmc_switch_boot_part(struct mmc_card *card, uint32_t boot_ack, uint32_t boot_part)
{
	return mmc_switch(card, MMC_EXT_CSD_CMD_SET_NORMAL, MMC_EXT_CSD_PART_CONF,
			  (card->extcsd.part_config & (~MMC_SWITCH_PART_BOOT_PART_MASK) & (~MMC_SWITCH_PART_BOOT_ACK_MASK))
			  | ((boot_part << 3) & MMC_SWITCH_PART_BOOT_PART_MASK) | (boot_ack << 6));
}

__xip_text
int32_t mmc_switch_boot_bus_cond(struct mmc_card *card, uint32_t boot_mode, uint32_t rst_bus_cond, uint32_t bus_width)
{
	return mmc_switch(card, MMC_EXT_CSD_CMD_SET_NORMAL, MMC_EXT_CSD_BOOT_BUS_COND,
	                  (card->extcsd.boot_bus_cond &
	                   (~MMC_SWITCH_BOOT_MODE_MASK) &
	                   (~MMC_SWITCH_BOOT_RST_BUS_COND_MASK) &
	                   (~MMC_SWITCH_BOOT_BUS_WIDTH_MASK))
	                  | ((boot_mode << 3) & MMC_SWITCH_BOOT_MODE_MASK)
	                  | ((rst_bus_cond << 2) & MMC_SWITCH_BOOT_RST_BUS_COND_MASK)
	                  | ((bus_width) & MMC_SWITCH_BOOT_BUS_WIDTH_MASK) );
}

__xip_text
int32_t mmc_app_cmd(struct mmc_host *host, struct mmc_card *card)
{
	int32_t err;
	struct mmc_command cmd = {0};

	SD_BUG_ON(!host);
	SD_BUG_ON(card && (card->host != host));

	cmd.opcode = MMC_APP_CMD;

	if (card) {
		cmd.arg = card->rca << 16;
		cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	} else {
		cmd.arg = 0;
		cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_BCR;
	}

	err = mmc_wait_for_cmd(host, &cmd);
	if (err)
		return err;

	/* Check that card supported application commands */
	if (!(cmd.resp[0] & R1_APP_CMD))
		return -1;

	return 0;
}

/**
 *	mmc_wait_for_app_cmd - start an application command and wait for
 *			       completion
 *	@host: MMC host to start command
 *	@card: Card to send MMC_APP_CMD to
 *	@cmd: MMC command to start
 *
 *	Sends a MMC_APP_CMD, checks the card response, sends the command
 *	in the parameter and waits for it to complete. Return any error
 *	that occurred while the command was executing.  Do not attempt to
 *	parse the response.
 */
__xip_text
int32_t mmc_wait_for_app_cmd(struct mmc_host *host, struct mmc_card *card,
                             struct mmc_command *cmd)
{
	struct mmc_request mrq = {NULL};

	int32_t i, err;

	SD_BUG_ON(!cmd);

	err = -1;

	/*
	 * We have to resend MMC_APP_CMD for each attempt so
	 * we cannot use the retries field in mmc_command.
	 */
	for (i = 0; i <= MMC_CMD_RETRIES; i++) {
		err = mmc_app_cmd(host, card);
		if (err) {
			continue;
		}

		memset(&mrq, 0, sizeof(struct mmc_request));

		memset(cmd->resp, 0, sizeof(cmd->resp));

		mrq.cmd = cmd;
		cmd->data = NULL;

		err = mmc_wait_for_req(host, &mrq);
		if (!err)
			break;
	}

	return err;
}

__xip_text
int32_t mmc_app_set_bus_width(struct mmc_card *card, uint32_t width)
{
	struct mmc_command cmd = {0};

	cmd.opcode = SET_BUS_WIDTH;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

	switch (width) {
		case MMC_BUS_WIDTH_1:
			cmd.arg = SD_BUS_WIDTH_1;
			break;
		case MMC_BUS_WIDTH_4:
			cmd.arg = SD_BUS_WIDTH_4;
			break;
		default:
			cmd.arg = SD_BUS_WIDTH_1;
	}

	if (mmc_wait_for_app_cmd(card->host, card, &cmd)) {
		return -1;
	}
	card->bus_width = width;

	return 0;
}

__xip_text
int32_t mmc_switch_to_high_speed(struct mmc_card *card)
{
	int32_t err;

#ifdef CONFIG_USE_MMC
	if (card->csd.mmc_spec_ver < MMC_CSD_SPEC_VER_4) {
		SD_LOGD("MMC card doesn't support to switch to high speed mode !!\n");
		return -1;
	}
#endif

	err = mmc_switch(card, MMC_EXT_CSD_CMD_SET_NORMAL, MMC_EXT_CSD_HS_TIMING, 1);
	if (err) {
		SD_LOGD("MMC card failed to switch to high speed mode !!\n");
		return err;
	}

	SD_LOGD("MMC card is switched to high speed!!\n");

	return 0;
}

__xip_text
int32_t smc_model_set_blkcnt(struct mmc_host *host, uint32_t blkcnt)
{
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_SET_BLOCK_COUNT;
	cmd.arg = blkcnt & 0xffff;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;

	if (mmc_wait_for_cmd(host, &cmd)) {
		return -1;
	}

	host->blkcnt = blkcnt;

	return 0;
}

__xip_text
int32_t __sdmmc_block_rw(struct mmc_card *card, uint32_t blk_num, uint32_t blk_cnt,
                           uint32_t sg_len, struct scatterlist *sg, int write)
{
	struct mmc_command cmd = {0};
	struct mmc_data data = {0};
	struct mmc_request mrq;
	uint32_t status = 0;

	SD_LOGD("%s %s blk_num:%d, blk_cnt:%d, sg_len:%d sg->len:%d\n", __func__,
	        write?"wirte":"read", blk_num, blk_cnt, sg_len, sg->len);

	if (blk_cnt > 1) {
		cmd.opcode = write ? MMC_WRITE_MULTIPLE_BLOCK : MMC_READ_MULTIPLE_BLOCK;
	} else {
		cmd.opcode = write ? MMC_WRITE_SINGLE_BLOCK : MMC_READ_SINGLE_BLOCK;
	}
	cmd.arg = blk_num;
	if (!mmc_card_blockaddr(card))
		cmd.arg <<= 9;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;
	cmd.stop = (blk_cnt == 1) ? 0 : 1;

	data.blksz = 512;
	data.flags = write ? MMC_DATA_WRITE : MMC_DATA_READ;

	data.sg = sg;
	data.sg_len = sg_len;

	mrq.cmd = &cmd;
	mrq.data = &data;

	SD_LOGD("starting CMD%d arg 0x%08x flags %x\n", cmd.opcode, cmd.arg, cmd.flags);
	SD_LOGD("  blksz %d blocks %d flags %x\n", data.blksz, data.blocks, data.flags);
	if (mmc_wait_for_req(card->host, &mrq)) {
		SD_LOGE("%s,%d %s sector:%x BSZ:%d Err!!\n", __func__, __LINE__,
			write?"W":"R", blk_num, blk_cnt);

		return -1;
	}
	if (write) {
		do {
			if (HAL_SDC_Is_Busy(card->host))
				continue;
			mmc_send_status(card, &status);
			mmc_mdelay(1);
		} while (!(status & 0x100));
	}
	return 0;
}

__xip_text
int32_t sdmmc_stream_write(struct mmc_card *card, uint32_t blk_num, uint32_t blk_size, uint32_t sg_len, struct scatterlist *sg)
{
	struct mmc_command cmd = {0};
	struct mmc_data data = {0};
	struct mmc_request mrq;
	uint32_t status = 0;

	cmd.opcode = MMC_WRITE_SINGLE_BLOCK;
	cmd.arg = blk_num;
	if (!mmc_card_blockaddr(card))
		cmd.arg <<= 9;
	cmd.stop = 0;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 |MMC_CMD_ADTC;
	cmd.data = &data;
	data.flags |= MMC_DATA_WRITE | MMC_DATA_STREAM;

	data.blksz = blk_size;
	data.sg_len = sg_len;
	data.sg = sg;
	mrq.cmd = &cmd;
	mrq.data = &data;
	if (mmc_wait_for_req(card->host, &mrq)) {
		return -1;
	}

	/* check busy */
	do {
		if (HAL_SDC_Is_Busy(card->host))
			continue;
		mmc_send_status(card, &status);
	} while (!(status & 0x100));
	return 0;
}
#endif

__xip_text
void mmc_add_card(struct mmc_card *card)
{
	uint8_t spec_ver = 0;
	uint32_t speed_hz = 0;
#if SD_DEBUG
	const char *speed_mode = "";
	const char *type;
#endif

	switch (card->type) {
#ifdef CONFIG_USE_MMC
	case MMC_TYPE_MMC:
		type = "MMC";
		if (card->csd.mmc_spec_ver == 4)
			spec_ver = 0x40;
		else
			spec_ver = 0x31;
		speed_hz = mmc_mmc_get_max_clock(card);
		break;
#endif
#ifdef CONFIG_USE_SD
	case MMC_TYPE_SD:
		type = "SD";
		if (mmc_card_blockaddr(card)) {
			if (mmc_card_ext_capacity(card))
				type = "SDXC";
			else
				type = "SDHC";
		}
		if (card->scr.sda_vsn == 0)
			spec_ver = 0x10;
		else if (card->scr.sda_vsn == 1)
			spec_ver = 0x11;
		else if (card->scr.sda_vsn == 2 && card->scr.sda_spec3)
			spec_ver = 0x30;
		else
			spec_ver = 0x20;
		if (card->scr.sda_vsn == 2 && card->scr.sda_spec3 && card->scr.sda_spec4)
			spec_ver = 0x40;
		if (card->scr.sda_vsn == 2 && card->scr.sda_spec3 && card->scr.sda_spec5)
			spec_ver = 0x50;
		speed_hz = mmc_sd_get_max_clock(card);
		break;
#endif
#ifdef CONFIG_USE_SDIO
	case MMC_TYPE_SDIO:
		type = "SDIO";
		spec_ver = 0x10;
		speed_hz = mmc_sdio_get_max_clock(card);
		break;
#endif
#ifdef CONFIG_USE_SD_COMBO
	case MMC_TYPE_SD_COMBO:
		type = "SD-combo";
		if (mmc_card_blockaddr(card))
			type = "SDHC-combo";
		break;
#endif
	default:
		type = "?";
		break;
	}

	if (card->sd_bus_speed == HIGH_SPEED_BUS_SPEED)
		speed_mode = "HS: 50 MHz";
	else
		speed_mode = "DS: 25 MHz";

	SD_LOGN("\n============= card information ==============\n");
	SD_LOGN("Card Type     : %s\n", type);
	SD_LOGN("Card Spec Ver : %x.%x\n", spec_ver>>4, spec_ver&0xf);
	SD_LOGN("Card RCA      : 0x%04x \n", card->rca);
	SD_LOGN("Card OCR      : 0x%x\n", card->ocr.ocr);
	SD_LOGN("    vol_window  : 0x%08x\n", card->ocr.vol_window);
	SD_LOGN("    to_1v8_acpt : %x\n", card->ocr.to_1v8_acpt);
	SD_LOGN("    high_capac  : %x\n", card->ocr.high_capacity);
	SD_LOGN("Card CSD      :\n");
	SD_LOGN("    speed       : %d KHz\n", speed_hz/1000);
#if ((defined CONFIG_USE_SD) || (defined CONFIG_USE_MMC))
	SD_LOGN("    cmd class   : 0x%x\n", card->csd.cmdclass);
	SD_LOGN("    capacity    : %dMB\n", card->csd.capacity/1024);
	SD_LOGN("Card CUR_STA  :\n");
	SD_LOGN("    speed_mode  : %s\n", speed_mode);
	SD_LOGN("    bus_width   : %d\n", card->bus_width);
	SD_LOGN("    speed_class : %d\n", card->speed_class);
#else
	(void)speed_mode;
#endif
	SD_LOGN("=============================================\n");
	(void)spec_ver;

	mmc_card_set_present(card);
}

#if ((defined CONFIG_USE_SD) || (defined CONFIG_USE_MMC))
__xip_text
int mmc_set_blocklen(struct mmc_card *card, unsigned int blocklen)
{
	struct mmc_command cmd = {0};

	if (mmc_card_blockaddr(card) || mmc_card_ddr_mode(card))
		return 0;

	cmd.opcode = MMC_SET_BLOCKLEN;
	cmd.arg = blocklen;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	return mmc_wait_for_cmd(card->host, &cmd);
}

/**
 * @brief read SD card.
 * @param card:
 *        @arg card->card handler.
 * @param buf:
 *        @arg buf->for store readed data.
 * @param sblk:
 *        @arg sblk->start block num.
 * @param nblk:
 *        @arg nblk->number of blocks.
 * @retval  0 if success or other if failed.
 */
int32_t mmc_block_read(struct mmc_card *card, uint8_t *buf, uint64_t sblk, uint32_t nblk)
{
	int32_t err;
	struct scatterlist sg = {0};

	SD_BUG_ON(!card->host);

	if (nblk > SDXC_MAX_TRANS_LEN/512) {
		SD_LOGW("%s only support len < %d\n", __func__, SDXC_MAX_TRANS_LEN/512);
		return -1;
	}

	mmc_claim_host(card->host);

	err = mmc_set_blocklen(card, 512);
	if (err)
		goto out;

	sg.len = 512 * nblk;
	sg.buffer = buf;
	//if ((unsigned int)buf & 0x03) {
	//	SD_LOGW("%s buf not align 4!!!\n", __func__);
	//	return -1;
	//}

	err = __sdmmc_block_rw(card, sblk, nblk, 1, &sg, 0);

out:
	mmc_release_host(card->host);
	return err;
}

/**
 * @brief write SD card.
 * @param card:
 *        @arg card->card handler.
 * @param buf:
 *        @arg buf->data will be write.
 * @param sblk:
 *        @arg sblk->start block num.
 * @param nblk:
 *        @arg nblk->number of blocks.
 * @retval  0 if success or other if failed.
 */
int32_t mmc_block_write(struct mmc_card *card, const uint8_t *buf, uint64_t sblk, uint32_t nblk)
{
	int32_t err;
	struct scatterlist sg = {0};

	SD_BUG_ON(!card->host);

	if (nblk > SDXC_MAX_TRANS_LEN/512) {
		SD_LOGW("%s only support len < %d\n", __func__, SDXC_MAX_TRANS_LEN/512);
		return -1;
	}

	mmc_claim_host(card->host);

	err = mmc_set_blocklen(card, 512);
	if (err)
		goto out;

	sg.len = 512 * nblk;
	sg.buffer = (uint8_t *)buf;
	//if ((unsigned int)buf & 0x03) {
	//	SD_LOGW("%s buf not align 4!!!\n", __func__);
	//	return -1;
	//}

	err = __sdmmc_block_rw(card, sblk, nblk, 1, &sg, 1);

out:
	mmc_release_host(card->host);
	return err;
}
#endif

__xip_text
int32_t mmc_send_relative_addr(struct mmc_host *host, uint32_t *rca)
{
	struct mmc_command cmd = {0};

	cmd.opcode = SD_SEND_RELATIVE_ADDR;
	cmd.arg = 0;
	cmd.flags = MMC_RSP_R6 | MMC_CMD_BCR;

	do {
		if (mmc_wait_for_cmd(host, &cmd)) {
			return -1;
		}
		*rca = cmd.resp[0] >> 16;
	} while (!*rca);

	return 0;
}

__xip_text
int32_t mmc_send_if_cond(struct mmc_host *host, uint32_t ocr)
{
	struct mmc_command cmd = {0};
	int32_t err;
	static const uint8_t test_pattern = 0xAA;
	uint8_t result_pattern;

	/*
	 * To support SD 2.0 cards, we must always invoke SD_SEND_IF_COND
	 * before SD_APP_OP_COND. This command will harmlessly fail for
	 * SD 1.0 cards.
	 */
	cmd.opcode = SD_SEND_IF_COND;
	cmd.arg = ((ocr & 0xFF8000) != 0) << 8 | test_pattern;
	cmd.flags = MMC_RSP_SPI_R7 | MMC_RSP_R7 | MMC_CMD_BCR;

	err = mmc_wait_for_cmd(host, &cmd);
	if (err)
		return err;

	result_pattern = cmd.resp[0] & 0xFF;

	if (result_pattern != test_pattern)
		return -1;

	return 0;
}

__xip_text
int32_t mmc_select_card(struct mmc_card *card, uint32_t select)
{
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_SELECT_CARD;
	if (select) {
		cmd.arg = card->rca << 16;
		cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;
	} else {
		cmd.arg = 0;
		cmd.flags = MMC_RSP_NONE | MMC_CMD_AC;
	}

	if (mmc_wait_for_cmd(card->host, &cmd)) {
		return -1;
	}

	return 0;
}

__xip_text
int32_t mmc_all_send_cid(struct mmc_host *host, uint32_t *cid)
{
	int32_t err;
	struct mmc_command cmd = {0};

	SD_BUG_ON(!host);
	SD_BUG_ON(!cid);

	cmd.opcode = MMC_ALL_SEND_CID;
	cmd.arg = 0;
	cmd.flags = MMC_RSP_R2 | MMC_CMD_BCR;

	err = mmc_wait_for_cmd(host, &cmd);
	if (err)
		return err;

	memcpy(cid, cmd.resp, sizeof(uint32_t) * 4);

	return 0;
}

#ifdef CONFIG_SD_PM
/*
 * Assign a mmc bus handler to a host. Only one bus handler may control a
 * host at any given time.
 */
__xip_text
void mmc_attach_bus(struct mmc_host *host, const struct mmc_bus_ops *ops)
{
	unsigned long flags;

	SD_BUG_ON(!host);
	SD_BUG_ON(!ops);

	flags = arch_irq_save();

	SD_BUG_ON(host->bus_ops);

	host->bus_ops = ops;

	arch_irq_restore(flags);
}

/*
 * Remove the current bus handler from a host.
 */
__xip_text
void mmc_detach_bus(struct mmc_host *host)
{
	unsigned long flags;

	SD_BUG_ON(!host);

	SD_WARN_ON(!host->bus_ops);

	flags = arch_irq_save();

	host->bus_ops = NULL;

	arch_irq_restore(flags);
}

#endif

/**
 * @brief scan or rescan SD card.
 * @param card:
 *        @arg card->card handler.
 * @param sdc_id:
 *        @arg sdc_id->SDC ID which card on.
 * @retval  0 if success or other if failed.
 */
__xip_text
int32_t mmc_rescan(struct mmc_card *card, uint32_t sdc_id)
{
	int32_t err = -1;
	struct mmc_host *host = _mci_host;

	if (!host) {
		SD_LOGE("%s init sdc host first!!\n", __func__);
		return -1;
	}

	card->host = host;

	mmc_claim_host(host);

	mmc_power_up(host);

	/* set identification clock 400KHz */
	HAL_SDC_Update_Clk(card->host, 400000);

	/* Initialization should be done at 3.3 V I/O voltage. */
	//mmc_set_signal_voltage(host, MMC_SIGNAL_VOLTAGE_330, 0);

	/*
	 * sdio_reset sends CMD52 to reset card.  Since we do not know
	 * if the card is being re-initialized, just send it.  CMD52
	 * should be ignored by SD/eMMC cards.
	 */
#ifdef CONFIG_USE_SDIO
	sdio_reset(host);
#endif
	mmc_go_idle(host);

	/* cmd8 for SD2.0 */
	if (mmc_send_if_cond(host, host->ocr_avail)) {
		SD_LOGN("sd1.0 or mmc\n");
	}

	/* Order's important: probe SDIO, then SD, then MMC */
#ifdef CONFIG_USE_SDIO
	SD_LOGN("***** Try sdio *****\n");
	if (!mmc_attach_sdio(card, host)){
		SD_LOGN("***** sdio init ok *****\n");
		err = 0;
		goto out;
	}
#endif
#ifdef CONFIG_USE_SD
	SD_LOGN("***** Try sd *****\n");
	if (!mmc_attach_sd(card, host)){
		SD_LOGN("***** sd init ok *****\n");
		err = 0;
		goto out;
	}
#endif
#ifdef CONFIG_USE_MMC
	SD_LOGN("***** Try mmc *****\n");
	if (!mmc_attach_mmc(card, host)){
		SD_LOGN("***** mmc init ok *****\n");
		err = 0;
		goto out;
	}
#endif

	SD_LOGD("Undown Card Detected!!\n");

	mmc_power_off(host);

out:
	mmc_release_host(host);
	return err;
}

/**
 * @brief deinit SD card.
 * @param card:
 *        @arg card->card handler.
 * @retval  0 if success or other if failed.
 */
__xip_text
int32_t mmc_card_deinit(struct mmc_card *card)
{
	SD_BUG_ON(!card->host);

#ifdef CONFIG_USE_SDIO
	mmc_deattach_sdio(card, card->host);
#endif
#ifdef CONFIG_USE_SD
	mmc_deattach_sd(card, card->host);
#endif

	mmc_power_off(card->host);
	memset(card, 0, sizeof(struct mmc_card));

	return 0;
}
