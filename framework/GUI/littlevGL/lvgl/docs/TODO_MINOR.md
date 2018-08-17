# TODOs for minor versions
Minor versions (x.1.0, x.2.0 ...) released when one or more new feature is addded without changing the API. New features can be added with major versions (1.0.0, 2.0.0 ...) too. 

## Contributing
Please create an issue to suggest a new feature instead of adding pull request to this file.

## Ideas
Here are ideas which are not assigned to a minor version yet:
- label: add a horzintal line (e.g. underline or line through).
- lv_split: new object type, a hor. or ver. line for decoration purpose
- lv_valset: new object type, a label with "+" and "-" buttons
- lv_listctrl: new object type, a list various controls on th right (sw, cb erc.)
- lv_inlist: new object type, inline drop down list (a button wich opens a list in place)
- lv_char: new_object type: characteristic set (like chart with draggable points)
- lv_vol: new_object type: volume meter (like a bar with segments)
- Anim. paths: monentum (tnh(x)), curve (exp), shake
- Hover, hover_lost signals
- Detached area (for video rendering where LittlevGL don't put pixels) 
- lv_ta: add placeholder text
- image rotate

## v5.3 (planned)
Mainly graphical/drawing improvments and Lua support
- [ ] API extension: turn the relevant "lv_obj" functions to the specific type (lv_btn_set_size)
- [ ] Lua interface to create GUI with script 
- [ ] Arabic glyph convert (based on letter position)
- [ ] Arc rawing
- [ ] Right-to-left write support 
- [ ] Bit based VDB: 1, 2 or 4 bit
- [ ] Ttriangle drawing

## v5.2 (in progress)
Mainly new object and new feauters:
- [ ] New object type: Listview (table) #137
- [ ] New object type: Calendar
- [ ] New object type: Icon (button like image) #182
- [ ] New object type: QR code #199
- [ ] lv_page: scroll on LV_GROUP_KEY_UP/DOWN/LEFT/RIGHT
- [ ] lv_obj_align: option in lv_conf.h sav the last alignment's coordinate ad aply it on lv_obj_realign
- [ ] lv_line: perpndicular line ending
- [ ] lv_gauge: option to put lables outside of the scale
- [ ] lv_img: png support #254
- [ ] lv_tabview: add option to put the tab button to the bottom
- [ ] Error callback: add an option to register a callback called on error
- [ ] Support more character coding (e.g. UTF8, UTF16 etc)

## v5.1 (released on: 09.03.2018)
- [x] lv_refr_set_roundig_callback: set a function to modify the invalidated area for special display controllers
- [x] lv_group_set_focus_callback: set function to call when a new object is focused #94
- [x] lv_obj_get_type() return string, e.g. "lv_slider", "lv_btn" etc #91
- [x] Font handling extension for effective Chiese font handling (cutsom read functions)
- [x] Remove LV_FONT_ANTIALIAS and add fonts with BPP (bit-per-pixel): 1, 2, 4 and 8
- [x] lv_img: add pixel level opacity option (ARGB8888 or ARGB8565) (make image upscale pointless)
- [x] LV_ANTIALIAS rework: meaning anti-alias lines and curves
- [x] Merge symbol fonts (basic, file and feedback) into one font 
- [x] lv_group: different default style_mod function with LV_COLOR_DEPTH   1
- [x] lv_img_set_src() to handle file path, symbols and const variables with one function
- [x] LV_PROTECT_PRESS_LOST: prevent the input devices to NOT find new object when the object's pressing is lost
- [x] lv_label: draw style.body.padding.hor/ver greater body if body_draw is enabled 
- [x] LV_LAYOUT_PRETTY: in one row align obeóject vertically to middle
- [x] Add user data option to lv_indev_drv_t and pass it with lv_indev_data_t to the read function. #115
- [x] LV_GROUP_KEY_ENTER_LONG: sent by the library on long press of LV_GROUP_KEY_ENTER to trigger long press of the object #113
- [x] LV_INDEV_TYPE_BUTTON: for a hatdware buttons which press a point on a screen

## v5.0 (released on: 20.12.2017)
- [x] UTF-8 support
- [x] lv_tabview: new object type to organise content with tabs
- [x] lv_sw: new object type, switch, turn on/off by tap (a little slider)
- [x] lv_roller: new object type, a roller to select a value (like on smartphones) 
- [x] lv_kb: new object type, Keyboard
- [x] lv_btnm: lv_btnm_set_tgl() to toggle last button
- [x] lv_ta: cursor types
- [x] add themes with predefined styles
- [x] partial border draw in styles

## v4.2 (released on: 17.08.2017)
- [x] Double VDB support: one for rendering, another to transfer former rendered image to frame buffer in the background (e.g. with DMA) [#15](https://github.com/littlevgl/lvgl/issues/15)
- [x] lv_group: to control without touch pad. Issue [#14](https://github.com/littlevgl/lvgl/issues/14)
- [x] lv_page: scrl def fit modification: hor:false, ver:true, and always set width to parent width
- [x] lv_btn: add lv_btn_get_..._action
- [x] lv_list: add lv_list_get_element_label/img
- [x] lv_ta: lv_ta_set_one_line to configure the Text area to one lined input field
- [x] style animations add
- [x] lv_btnm:  besides 0. byte (width dsc) 1. byte: hidden (\177, 0x7F, delete)
