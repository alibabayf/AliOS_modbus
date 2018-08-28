

NAME := cedarx

$(NAME)_TYPE := kernel

cedarx_with_pribuild_lib=1

ifneq ($(cedarx_with_pribuild_lib),1)
$(NAME)_SOURCES := awrecorder/AudioEncodeComponent.c \
                   awrecorder/awencoder.c \
                   awrecorder/EncDataComponent.c \
                   base/AwMessageQueue.c \
                   base/AwPool.c \
                   base/CdxBitReader.c \
                   base/CdxBuffer.c \
                   base/CdxDebug.c \
                   base/CdxISOLang.c \
                   base/CdxKeyedVector.c \
                   base/CdxList.c \
                   base/CdxMessage.c \
                   base/CdxMeta.c \
                   base/CdxParseUrl.c \
                   base/CdxQueue.c \
                   base/CdxSocketUtil.c \
                   base/CdxStrUtil.c \
                   base/CdxSysinfo.c \
                   base/CdxTime.c \
                   base/cdx_log.c \
                   base/SmartDnsService.c \
                   Cdx2.0Plugin/adecoder.c \
                   Cdx2.0Plugin/src/AudioDec_Decode.c \
                   Cdx2.0Plugin/src/cedar_abs_packet_hdr.c \
                   Cdx2.0Plugin/src/rw_data_api.c \
                   CdxEncPlugin/aencoder.c \
                   CdxEncPlugin/pcm_enc.c \
                   muxer/aac/CdxAacMuxer.c \
                   muxer/base/CdxMuxer.c \
                   muxer/base/CdxTsemaphore.c \
                   muxer/RecoderCbWriter.c \
                   muxer/RecoderWriter.c \
                   os_glue/atomic.c \
                   os_glue/file.c \
                   os_glue/pthread.c \
                   os_glue/sleep.c \
                   parser/amr/CdxAmrParser.c \
                   parser/base/CdxParser.c \
                   parser/base/id3base/CdxMetaData.c \
                   parser/base/id3base/CdxUtfCode.c \
                   parser/base/id3base/Id3Base.c \
                   parser/base/id3base/StringContainer.c \
                   parser/id3v2/CdxId3v2Parser.c \
                   parser/mp3/CdxMp3Parser.c \
                   playback/audioDecComponent.c \
                   playback/audioRenderComponent.c \
                   playback/avtimer.c \
                   playback/baseComponent.c \
                   playback/bitrateEstimater.c \
                   playback/framerateEstimater.c \
                   playback/player.c \
                   playback/soundControl_null.c \
                   playback/soundControl_rtos.c \
                   playback/streamManager.c \
                   record/captureControl_null.c \
                   record/captureControl_rtos.c \
                   stream/base/CdxStream.c \
                   stream/file/CdxFileStream.c \
                   stream/flash/CdxFlashStream.c \
                   stream/http/CdxHttpField.c \
                   stream/http/CdxHttpStream.c \
                   stream/http/CdxHttpUtils.c \
                   stream/tcp/CdxTcpStream.c \
                   xplayer/cache.c \
                   xplayer/demuxComponent.c \
                   xplayer/xplayer.c \
                   xrecoder/xrecoder.c
endif

$(NAME)_INCLUDES := ./
$(NAME)_INCLUDES += awrecorder
$(NAME)_INCLUDES += base
$(NAME)_INCLUDES += base/include
$(NAME)_INCLUDES += Cdx2.0Plugin
$(NAME)_INCLUDES += Cdx2.0Plugin/include
$(NAME)_INCLUDES += Cdx2.0Plugin/src
$(NAME)_INCLUDES += Cdx2.0Plugin/tools
$(NAME)_INCLUDES += Cdx2.0Plugin/tools/memcheck
$(NAME)_INCLUDES += CdxEncPlugin
$(NAME)_INCLUDES += CdxEncPlugin/include
$(NAME)_INCLUDES += common
$(NAME)_INCLUDES += common/iniparser
$(NAME)_INCLUDES += external
$(NAME)_INCLUDES += external/include
$(NAME)_INCLUDES += external/include/adecoder
$(NAME)_INCLUDES += external/include/aencoder
$(NAME)_INCLUDES += external/include/zlib
$(NAME)_INCLUDES += muxer
$(NAME)_INCLUDES += muxer/aac
$(NAME)_INCLUDES += muxer/base
$(NAME)_INCLUDES += muxer/include
$(NAME)_INCLUDES += os_glue
$(NAME)_INCLUDES += os_glue/include
$(NAME)_INCLUDES += parser
$(NAME)_INCLUDES += parser/amr
$(NAME)_INCLUDES += parser/base
$(NAME)_INCLUDES += parser/base/id3base
$(NAME)_INCLUDES += parser/id3v2
$(NAME)_INCLUDES += parser/include
$(NAME)_INCLUDES += parser/mp3
$(NAME)_INCLUDES += playback
$(NAME)_INCLUDES += playback/include
$(NAME)_INCLUDES += record
$(NAME)_INCLUDES += record/include
$(NAME)_INCLUDES += stream
$(NAME)_INCLUDES += stream/base
$(NAME)_INCLUDES += stream/file
$(NAME)_INCLUDES += stream/flash
$(NAME)_INCLUDES += stream/http
$(NAME)_INCLUDES += stream/include
$(NAME)_INCLUDES += stream/tcp
$(NAME)_INCLUDES += xplayer
$(NAME)_INCLUDES += xplayer/include
$(NAME)_INCLUDES += xrecoder
$(NAME)_INCLUDES += xrecoder/include

ifeq ($(cedarx_with_pribuild_lib),1)
$(NAME)_PREBUILT_LIBRARY := cedarx.a
endif
