// #include "lv_mainstart.h"
// #include "lvgl/examples/lv_examples.h"
// #include "lvgl/src/lv_conf_internal.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// /* 2048 logic */

// /* 获取当前活动屏幕的宽高 */
// #define scr_act_width() lv_obj_get_width(lv_scr_act())	 // 800
// #define scr_act_height() lv_obj_get_height(lv_scr_act()) // 480

// /* 游戏变量 */
// // 棋盘状态（4×4数组，0表示空，非0表示数字）
// static int matrix[4][4] = {0};
// // 数字块对象数组，用于关联界面和逻辑
// static lv_obj_t *tiles[4][4] = {0};
// // 画布
// static lv_obj_t *canvas;	// 棋盘画布（全局，供格子初始化使用）
// static lv_obj_t *msg_label; // 胜负提示标签
// // 自定义胜负提示标签的样式
// static lv_style_t style_msg_label;
// // 声明所有样式（全局）
// static lv_style_t style_tile_0;	   // 空格子
// static lv_style_t style_tile_2;	   // 数字2
// static lv_style_t style_tile_4;	   // 数字4
// static lv_style_t style_tile_8;	   // 数字8
// static lv_style_t style_tile_16;   // 数字16
// static lv_style_t style_tile_32;   // 数字32
// static lv_style_t style_tile_64;   // 数字64
// static lv_style_t style_tile_128;  // 数字128
// static lv_style_t style_tile_256;  // 数字256
// static lv_style_t style_tile_512;  // 数字512
// static lv_style_t style_tile_1024; // 数字1024
// static lv_style_t style_tile_2048; // 数字2048
// /* 函数声明 */
// void draw_square_grid(uint16_t big_square_side); // 画棋盘
// void init_tile_styles(void);					 // 生成所有样式
// void init_tiles(void);							 // 初始化4×4格子，位置在棋盘内
// void lvgl_2048_btn();							 // 画按钮
// void update_board_ui(void);						 // 同步界面与matrix数据
// int addRandomNumber(void);						 // 随机生成数字
// int moveLeft(void);								 // 左移
// int moveRight(void);							 // 右移
// int moveUp(void);								 // 上移
// int moveDown(void);								 // 下移
// int isGameOver(void);							 // 判断游戏结束
// int checkWin(void);								 // 判断胜利
// void show_message(const char *msg);				 // 显示提示信息
// /* 外部变量声明 */
// int number = 0;

// /* 主函数 */
// void lv_mainstart(void)
// {
// 	srand(time(NULL)); // 初始化随机数种子
// 	init_tile_styles();
// 	draw_square_grid(360);
// 	lvgl_2048_btn();
// 	init_tiles();
// 	// 创建胜负提示标签（初始隐藏）
// 	msg_label = lv_label_create(lv_scr_act());
// 	lv_obj_add_style(msg_label, &style_msg_label, 0); // 使用自定义样式
// 	lv_obj_align(msg_label, LV_ALIGN_TOP_MID, 0, 20);
// 	lv_label_set_text(msg_label, "");

// 	// 初始生成两个数字
// 	addRandomNumber();
// 	addRandomNumber();
// 	update_board_ui(); // 同步界面
// }

// void draw_square_grid(uint16_t big_square_side)
// {
// 	/* 整体画布 */
// 	static lv_color_t canvas_buf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(450, 450)];	  // 200x150真彩色缓冲区
// 	canvas = lv_canvas_create(lv_scr_act());								  // 在屏幕创建画布
// 	lv_canvas_set_buffer(canvas, canvas_buf, 400, 400, LV_IMG_CF_TRUE_COLOR); // 绑定缓冲区
// 	// 4. 设置画布左上角坐标为（x=20, y=50）
// 	// lv_obj_set_pos(canvas, 100, 50); // 关键：直接设置对象位置
// 	lv_obj_center(canvas);

// 	/* 棋盘 */
// 	// 2. 初始化矩形绘制描述符（关键：必须先init）
// 	lv_draw_rect_dsc_t rect_dsc;
// 	lv_draw_rect_dsc_init(&rect_dsc);

// 	// 3. 配置矩形样式（按需调整，以下为常用属性）
// 	rect_dsc.radius = 8;						// 圆角半径（0=直角）
// 	rect_dsc.bg_color = lv_color_hex(0x03A9F4); // 背景色（青色）
// 	rect_dsc.bg_opa = LV_OPA_80;				// 背景不透明度（80%，0=透明，255=完全不透明）
// 	rect_dsc.border_width = 2;					// 边框宽度（0=无边框）
// 	rect_dsc.border_color = lv_color_white();	// 边框颜色（白色）
// 	rect_dsc.shadow_width = 0;					// 阴影宽度（0=无阴影）
// 	rect_dsc.shadow_ofs_x = 0;					// 阴影X轴偏移
// 	rect_dsc.shadow_ofs_y = 0;					// 阴影Y轴偏移

// 	// 4. 绘制矩形（x=20, y=20：矩形左上角坐标（相对坐标canvas）；w=360, h=360：矩形宽高）
// 	lv_canvas_draw_rect(canvas, 20, 20, big_square_side, big_square_side, &rect_dsc);

// 	/* 棋盘上的线条 */
// 	// 绘制3条竖线和3条横线，划分16个小正方形
// 	lv_draw_line_dsc_t line_dsc;
// 	lv_draw_line_dsc_init(&line_dsc);  // 初始化线条描述符
// 	line_dsc.color = lv_color_white(); // 线条颜色（白色，与边框一致）
// 	line_dsc.width = 2;				   // 线条宽度
// 	line_dsc.opa = LV_OPA_COVER;	   // 完全不透明

// 	// 定义矩形的边界（与外框矩形参数对应，方便计算）
// 	int x0 = 20;					// 矩形左上角x（相对画布）
// 	int y0 = 20;					// 矩形左上角y（相对画布）
// 	int w = big_square_side;		// 矩形宽度
// 	int h = big_square_side;		// 矩形高度
// 	int step = big_square_side / 4; // 每等份的尺寸（360/4=90）

// 	// 3条竖线：垂直方向贯穿矩形（x固定，y从y0到y0+h）
// 	lv_point_t line_ver1[] = {{x0 + step * 1, y0}, {x0 + step * 1, y0 + h}}; // 第1条竖线
// 	lv_point_t line_ver2[] = {{x0 + step * 2, y0}, {x0 + step * 2, y0 + h}}; // 第2条竖线
// 	lv_point_t line_ver3[] = {{x0 + step * 3, y0}, {x0 + step * 3, y0 + h}}; // 第3条竖线

// 	// 3条横线：水平方向贯穿矩形（y固定，x从x0到x0+w）
// 	lv_point_t line_hor1[] = {{x0, y0 + step * 1}, {x0 + w, y0 + step * 1}}; // 第1条横线
// 	lv_point_t line_hor2[] = {{x0, y0 + step * 2}, {x0 + w, y0 + step * 2}}; // 第2条横线
// 	lv_point_t line_hor3[] = {{x0, y0 + step * 3}, {x0 + w, y0 + step * 3}}; // 第3条横线

// 	// 绘制所有线条（每个线条由2个点组成，因此cnt=2）
// 	lv_canvas_draw_line(canvas, line_ver1, 2, &line_dsc);
// 	lv_canvas_draw_line(canvas, line_ver2, 2, &line_dsc);
// 	lv_canvas_draw_line(canvas, line_ver3, 2, &line_dsc);
// 	lv_canvas_draw_line(canvas, line_hor1, 2, &line_dsc);
// 	lv_canvas_draw_line(canvas, line_hor2, 2, &line_dsc);
// 	lv_canvas_draw_line(canvas, line_hor3, 2, &line_dsc);
// }

// /* 画按钮 */

// /* 按钮事件回调（修改：触发移动并更新界面） */
// static void event_handler(lv_event_t *e)
// {
// 	lv_event_code_t code = lv_event_get_code(e);
// 	lv_obj_t *btn = lv_event_get_target(e);
// 	if (code == LV_EVENT_CLICKED)
// 	{
// 		const char *btn_name = lv_obj_get_user_data(btn);
// 		LV_LOG_USER("%s was clicked", btn_name);
// 		int moved = 0;

// 		// 根据按钮名称触发对应移动
// 		if (strcmp(btn_name, "Up") == 0)
// 		{
// 			moved = moveUp();
// 		}
// 		else if (strcmp(btn_name, "Down") == 0)
// 		{
// 			moved = moveDown();
// 		}
// 		else if (strcmp(btn_name, "Left") == 0)
// 		{
// 			moved = moveLeft();
// 		}
// 		else if (strcmp(btn_name, "Right") == 0)
// 		{
// 			moved = moveRight();
// 		}

// 		// 移动后更新界面并生成新数字
// 		if (moved)
// 		{
// 			addRandomNumber();
// 			update_board_ui();

// 			// 检查胜负
// 			if (checkWin())
// 			{
// 				show_message("恭喜！你赢了！");
// 			}
// 			else if (isGameOver())
// 			{
// 				show_message("游戏结束！");
// 			}
// 			else
// 			{
// 				show_message(""); // 清除提示
// 			}
// 		}
// 	}
// }
// void lvgl_2048_btn(void)
// {
// 	lv_obj_t *label;

// 	// Up按钮
// 	lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
// 	lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
// 	lv_obj_align(btn1, LV_ALIGN_CENTER, 300, 0);
// 	lv_obj_set_user_data(btn1, "Up"); // 绑定标识"Up"

// 	label = lv_label_create(btn1);
// 	lv_label_set_text(label, "Up");
// 	lv_obj_center(label);

// 	// Right按钮
// 	lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
// 	lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
// 	lv_obj_align(btn2, LV_ALIGN_CENTER, 350, 50);
// 	lv_obj_set_user_data(btn2, "Right"); // 绑定标识"Right"

// 	label = lv_label_create(btn2);
// 	lv_label_set_text(label, "Right");
// 	lv_obj_center(label);

// 	// Left按钮
// 	lv_obj_t *btn3 = lv_btn_create(lv_scr_act());
// 	lv_obj_add_event_cb(btn3, event_handler, LV_EVENT_ALL, NULL);
// 	lv_obj_align(btn3, LV_ALIGN_CENTER, 250, 50);
// 	lv_obj_set_user_data(btn3, "Left"); // 绑定标识"Left"

// 	label = lv_label_create(btn3);
// 	lv_label_set_text(label, "Left");
// 	lv_obj_center(label);

// 	// Down按钮
// 	lv_obj_t *btn4 = lv_btn_create(lv_scr_act());
// 	lv_obj_add_event_cb(btn4, event_handler, LV_EVENT_ALL, NULL);
// 	lv_obj_align(btn4, LV_ALIGN_CENTER, 300, 100);
// 	lv_obj_set_user_data(btn4, "Down"); // 绑定标识"Down"

// 	label = lv_label_create(btn4);
// 	lv_label_set_text(label, "Down");
// 	lv_obj_center(label);
// }

// /* 格子 */
// // 生成所有样式
// void init_tile_styles(void)
// {
// 	// 在 init_tile_styles 函数末尾添加
// 	// 初始化提示标签样式
// 	lv_style_init(&style_msg_label);
// 	lv_style_set_text_font(&style_msg_label, &lv_font_montserrat_24);  // 字体大小
// 	lv_style_set_text_color(&style_msg_label, lv_color_hex(0xff0000)); // 红色文字（醒目）
// 	lv_style_set_bg_opa(&style_msg_label, LV_OPA_TRANSP);			   // 透明背景
// 	// 空格子（数字0）：深灰色背景，无文字
// 	lv_style_init(&style_tile_0);
// 	lv_style_set_bg_color(&style_tile_0, lv_color_hex(0x03A9F4));
// 	lv_style_set_radius(&style_tile_0, 4);
// 	lv_style_set_border_width(&style_tile_0, 0);
// 	lv_style_set_pad_all(&style_tile_0, 0);

// 	// 数字2：浅灰色背景，深灰文字
// 	lv_style_init(&style_tile_2);
// 	lv_style_set_bg_color(&style_tile_2, lv_color_hex(0xeee4da));
// 	lv_style_set_text_color(&style_tile_2, lv_color_hex(0x776e65));
// 	lv_style_set_text_font(&style_tile_2, &lv_font_montserrat_18);
// 	lv_style_set_radius(&style_tile_2, 4);
// 	lv_style_set_border_width(&style_tile_2, 0);
// 	lv_style_set_pad_all(&style_tile_2, 8);

// 	// 数字4：米色背景，深灰文字
// 	lv_style_init(&style_tile_4);
// 	lv_style_set_bg_color(&style_tile_4, lv_color_hex(0xede0c8));
// 	lv_style_set_text_color(&style_tile_4, lv_color_hex(0x776e65));
// 	lv_style_set_text_font(&style_tile_4, &lv_font_montserrat_18);
// 	lv_style_set_radius(&style_tile_4, 4);
// 	lv_style_set_border_width(&style_tile_4, 0);
// 	lv_style_set_pad_all(&style_tile_4, 8);

// 	// 数字8：浅橙色背景，白色文字
// 	lv_style_init(&style_tile_8);
// 	lv_style_set_bg_color(&style_tile_8, lv_color_hex(0xf2b179));
// 	lv_style_set_text_color(&style_tile_8, lv_color_white());
// 	lv_style_set_text_font(&style_tile_8, &lv_font_montserrat_18);
// 	lv_style_set_radius(&style_tile_8, 4);
// 	lv_style_set_border_width(&style_tile_8, 0);
// 	lv_style_set_pad_all(&style_tile_8, 8);

// 	// 数字16：橙色背景，白色文字
// 	lv_style_init(&style_tile_16);
// 	lv_style_set_bg_color(&style_tile_16, lv_color_hex(0xf59563));
// 	lv_style_set_text_color(&style_tile_16, lv_color_white());
// 	lv_style_set_text_font(&style_tile_16, &lv_font_montserrat_18);
// 	lv_style_set_radius(&style_tile_16, 4);
// 	lv_style_set_border_width(&style_tile_16, 0);
// 	lv_style_set_pad_all(&style_tile_16, 8);

// 	// 数字32：深橙色背景，白色文字
// 	lv_style_init(&style_tile_32);
// 	lv_style_set_bg_color(&style_tile_32, lv_color_hex(0xf67c5f));
// 	lv_style_set_text_color(&style_tile_32, lv_color_white());
// 	lv_style_set_text_font(&style_tile_32, &lv_font_montserrat_18);
// 	lv_style_set_radius(&style_tile_32, 4);
// 	lv_style_set_border_width(&style_tile_32, 0);
// 	lv_style_set_pad_all(&style_tile_32, 8);

// 	// 数字64：红色背景，白色文字
// 	lv_style_init(&style_tile_64);
// 	lv_style_set_bg_color(&style_tile_64, lv_color_hex(0xf65e3b));
// 	lv_style_set_text_color(&style_tile_64, lv_color_white());
// 	lv_style_set_text_font(&style_tile_64, &lv_font_montserrat_18);
// 	lv_style_set_radius(&style_tile_64, 4);
// 	lv_style_set_border_width(&style_tile_64, 0);
// 	lv_style_set_pad_all(&style_tile_64, 8);

// 	// 数字128：浅黄背景，白色文字
// 	lv_style_init(&style_tile_128);
// 	lv_style_set_bg_color(&style_tile_128, lv_color_hex(0xedcf72));
// 	lv_style_set_text_color(&style_tile_128, lv_color_white());
// 	lv_style_set_text_font(&style_tile_128, &lv_font_montserrat_16);
// 	lv_style_set_radius(&style_tile_128, 4);
// 	lv_style_set_border_width(&style_tile_128, 0);
// 	lv_style_set_pad_all(&style_tile_128, 6);

// 	// 数字256：浅黄背景，白色文字
// 	lv_style_init(&style_tile_256);
// 	lv_style_set_bg_color(&style_tile_256, lv_color_hex(0xedcc61));
// 	lv_style_set_text_color(&style_tile_256, lv_color_white());
// 	lv_style_set_text_font(&style_tile_256, &lv_font_montserrat_16);
// 	lv_style_set_radius(&style_tile_256, 4);
// 	lv_style_set_border_width(&style_tile_256, 0);
// 	lv_style_set_pad_all(&style_tile_256, 6);

// 	// 数字512：浅黄背景，白色文字
// 	lv_style_init(&style_tile_512);
// 	lv_style_set_bg_color(&style_tile_512, lv_color_hex(0xedc850));
// 	lv_style_set_text_color(&style_tile_512, lv_color_white());
// 	lv_style_set_text_font(&style_tile_512, &lv_font_montserrat_16);
// 	lv_style_set_radius(&style_tile_512, 4);
// 	lv_style_set_border_width(&style_tile_512, 0);
// 	lv_style_set_pad_all(&style_tile_512, 6);

// 	// 数字1024：黄色背景，白色文字
// 	lv_style_init(&style_tile_1024);
// 	lv_style_set_bg_color(&style_tile_1024, lv_color_hex(0xedc53f));
// 	lv_style_set_text_color(&style_tile_1024, lv_color_white());
// 	lv_style_set_text_font(&style_tile_1024, &lv_font_montserrat_14);
// 	lv_style_set_radius(&style_tile_1024, 4);
// 	lv_style_set_border_width(&style_tile_1024, 0);
// 	lv_style_set_pad_all(&style_tile_1024, 6);

// 	// 数字2048：红色背景，白色文字
// 	lv_style_init(&style_tile_2048);
// 	lv_style_set_bg_color(&style_tile_2048, lv_color_hex(0x3c3a32));
// 	lv_style_set_text_color(&style_tile_2048, lv_color_white());
// 	lv_style_set_text_font(&style_tile_2048, &lv_font_montserrat_14);
// 	lv_style_set_radius(&style_tile_2048, 4);
// 	lv_style_set_border_width(&style_tile_2048, 0);
// 	lv_style_set_pad_all(&style_tile_2048, 6);
// }

// // 初始化4×4格子，位置在棋盘内
// void init_tiles(void)
// {
// 	int tile_size = 80;	  // 每个格子的大小（需与棋盘线条布局匹配）
// 	int gap = 10;		  // 格子之间的间隙
// 	int board_x = 20 + 5; // 棋盘在画布内的左上角x坐标
// 	int board_y = 20 + 5; // 棋盘在画布内的左上角y坐标

// 	for (int i = 0; i < 4; i++)
// 	{
// 		for (int j = 0; j < 4; j++)
// 		{
// 			// 在画布上创建格子对象
// 			tiles[i][j] = lv_obj_create(canvas);
// 			lv_obj_set_size(tiles[i][j], tile_size, tile_size);
// 			// 计算格子在棋盘内的位置：board_x + j*(tile_size+gap), board_y + i*(tile_size+gap)
// 			lv_obj_set_pos(tiles[i][j],
// 						   board_x + j * (tile_size + gap),
// 						   board_y + i * (tile_size + gap));
// 			// 初始样式：数字0（空）
// 			lv_obj_add_style(tiles[i][j], &style_tile_0, 0);
// 		}
// 	}
// }

// /* 随机生成2 */
// int addRandomNumber(void)
// {
// 	int emptyCells = 0;
// 	for (int i = 0; i < 4; i++)
// 	{
// 		for (int j = 0; j < 4; j++)
// 		{
// 			if (matrix[i][j] == 0)
// 				emptyCells++;
// 		}
// 	}
// 	if (emptyCells == 0)
// 		return 0;

// 	int randomIndex = rand() % emptyCells;
// 	int count = 0;
// 	for (int i = 0; i < 4; i++)
// 	{
// 		for (int j = 0; j < 4; j++)
// 		{
// 			if (matrix[i][j] == 0)
// 			{
// 				if (count == randomIndex)
// 				{
// 					matrix[i][j] = 2;
// 					return 1;
// 				}
// 				count++;
// 			}
// 		}
// 	}
// 	return 0;
// }

// /* 左移逻辑（复用控制台逻辑） */
// int moveLeft(void)
// {
// 	int moved = 0;
// 	for (int i = 0; i < 4; i++)
// 	{
// 		for (int j = 1; j < 4; j++)
// 		{
// 			int k = j;
// 			// 移动到最左非空位置
// 			while (k > 0 && matrix[i][k - 1] == 0)
// 			{
// 				matrix[i][k - 1] = matrix[i][k];
// 				matrix[i][k] = 0;
// 				k--;
// 				moved = 1;
// 			}
// 			// 合并相同数字
// 			if (k > 0 && matrix[i][k - 1] == matrix[i][k])
// 			{
// 				matrix[i][k - 1] *= 2;
// 				matrix[i][k] = 0;
// 				moved = 1;
// 			}
// 		}
// 	}
// 	return moved;
// }

// /* 右移逻辑 */
// int moveRight(void)
// {
// 	int moved = 0;
// 	for (int i = 0; i < 4; i++)
// 	{
// 		for (int j = 2; j >= 0; j--)
// 		{
// 			int k = j;
// 			while (k < 3 && matrix[i][k + 1] == 0)
// 			{
// 				matrix[i][k + 1] = matrix[i][k];
// 				matrix[i][k] = 0;
// 				k++;
// 				moved = 1;
// 			}
// 			if (k < 3 && matrix[i][k + 1] == matrix[i][k])
// 			{
// 				matrix[i][k + 1] *= 2;
// 				matrix[i][k] = 0;
// 				moved = 1;
// 			}
// 		}
// 	}
// 	return moved;
// }

// /* 上移逻辑 */
// int moveUp(void)
// {
// 	int moved = 0;
// 	for (int j = 0; j < 4; j++)
// 	{
// 		for (int i = 1; i < 4; i++)
// 		{
// 			int k = i;
// 			while (k > 0 && matrix[k - 1][j] == 0)
// 			{
// 				matrix[k - 1][j] = matrix[k][j];
// 				matrix[k][j] = 0;
// 				k--;
// 				moved = 1;
// 			}
// 			if (k > 0 && matrix[k - 1][j] == matrix[k][j])
// 			{
// 				matrix[k - 1][j] *= 2;
// 				matrix[k][j] = 0;
// 				moved = 1;
// 			}
// 		}
// 	}
// 	return moved;
// }

// /* 下移逻辑 */
// int moveDown(void)
// {
// 	int moved = 0;
// 	for (int j = 0; j < 4; j++)
// 	{
// 		for (int i = 2; i >= 0; i--)
// 		{
// 			int k = i;
// 			while (k < 3 && matrix[k + 1][j] == 0)
// 			{
// 				matrix[k + 1][j] = matrix[k][j];
// 				matrix[k][j] = 0;
// 				k++;
// 				moved = 1;
// 			}
// 			if (k < 3 && matrix[k + 1][j] == matrix[k][j])
// 			{
// 				matrix[k + 1][j] *= 2;
// 				matrix[k][j] = 0;
// 				moved = 1;
// 			}
// 		}
// 	}
// 	return moved;
// }

// /* 判断游戏结束 */
// int isGameOver(void)
// {
// 	// 有空位则未结束
// 	for (int i = 0; i < 4; i++)
// 	{
// 		for (int j = 0; j < 4; j++)
// 		{
// 			if (matrix[i][j] == 0)
// 				return 0;
// 		}
// 	}
// 	// 有相邻相同数字则未结束
// 	for (int i = 0; i < 4; i++)
// 	{
// 		for (int j = 0; j < 4; j++)
// 		{
// 			if (i < 3 && matrix[i][j] == matrix[i + 1][j])
// 				return 0;
// 			if (j < 3 && matrix[i][j] == matrix[i][j + 1])
// 				return 0;
// 		}
// 	}
// 	return 1;
// }

// /* 判断胜利（出现2048） */
// int checkWin(void)
// {
// 	for (int i = 0; i < 4; i++)
// 	{
// 		for (int j = 0; j < 4; j++)
// 		{
// 			if (matrix[i][j] == 2048)
// 				return 1;
// 		}
// 	}
// 	return 0;
// }

// /* 显示胜负提示 */
// void show_message(const char *msg)
// {
// 	lv_label_set_text(msg_label, msg);
// }
// /* 同步界面与matrix数据：根据matrix更新格子样式和文本 */
// void update_board_ui(void)
// {
// 	for (int i = 0; i < 4; i++)
// 	{
// 		for (int j = 0; j < 4; j++)
// 		{
// 			// 清除原有样式和子对象（避免文本叠加）
// 			lv_obj_remove_style_all(tiles[i][j]);
// 			lv_obj_clean(tiles[i][j]);

// 			// 根据matrix值设置样式和文本
// 			switch (matrix[i][j])
// 			{
// 			case 0:
// 				lv_obj_add_style(tiles[i][j], &style_tile_0, 0);
// 				break;
// 			case 2:
// 				lv_obj_add_style(tiles[i][j], &style_tile_2, 0);
// 				lv_label_create(tiles[i][j]);
// 				lv_label_set_text(lv_obj_get_child(tiles[i][j], 0), "2");
// 				lv_obj_center(lv_obj_get_child(tiles[i][j], 0));
// 				break;
// 			case 4:
// 				lv_obj_add_style(tiles[i][j], &style_tile_4, 0);
// 				lv_label_create(tiles[i][j]);
// 				lv_label_set_text(lv_obj_get_child(tiles[i][j], 0), "4");
// 				lv_obj_center(lv_obj_get_child(tiles[i][j], 0));
// 				break;
// 			case 8:
// 				lv_obj_add_style(tiles[i][j], &style_tile_8, 0);
// 				lv_label_create(tiles[i][j]);
// 				lv_label_set_text(lv_obj_get_child(tiles[i][j], 0), "8");
// 				lv_obj_center(lv_obj_get_child(tiles[i][j], 0));
// 				break;
// 			case 16:
// 				lv_obj_add_style(tiles[i][j], &style_tile_16, 0);
// 				lv_label_create(tiles[i][j]);
// 				lv_label_set_text(lv_obj_get_child(tiles[i][j], 0), "16");
// 				lv_obj_center(lv_obj_get_child(tiles[i][j], 0));
// 				break;
// 			case 32:
// 				lv_obj_add_style(tiles[i][j], &style_tile_32, 0);
// 				lv_label_create(tiles[i][j]);
// 				lv_label_set_text(lv_obj_get_child(tiles[i][j], 0), "32");
// 				lv_obj_center(lv_obj_get_child(tiles[i][j], 0));
// 				break;
// 			case 64:
// 				lv_obj_add_style(tiles[i][j], &style_tile_64, 0);
// 				lv_label_create(tiles[i][j]);
// 				lv_label_set_text(lv_obj_get_child(tiles[i][j], 0), "64");
// 				lv_obj_center(lv_obj_get_child(tiles[i][j], 0));
// 				break;
// 			case 128:
// 				lv_obj_add_style(tiles[i][j], &style_tile_128, 0);
// 				lv_label_create(tiles[i][j]);
// 				lv_label_set_text(lv_obj_get_child(tiles[i][j], 0), "128");
// 				lv_obj_center(lv_obj_get_child(tiles[i][j], 0));
// 				break;
// 			case 256:
// 				lv_obj_add_style(tiles[i][j], &style_tile_256, 0);
// 				lv_label_create(tiles[i][j]);
// 				lv_label_set_text(lv_obj_get_child(tiles[i][j], 0), "256");
// 				lv_obj_center(lv_obj_get_child(tiles[i][j], 0));
// 				break;
// 			case 1024:
// 				lv_obj_add_style(tiles[i][j], &style_tile_1024, 0);
// 				lv_label_create(tiles[i][j]);
// 				lv_label_set_text(lv_obj_get_child(tiles[i][j], 0), "1024");
// 				lv_obj_center(lv_obj_get_child(tiles[i][j], 0));
// 				break;
// 			case 2048:
// 				lv_obj_add_style(tiles[i][j], &style_tile_2048, 0);
// 				lv_label_create(tiles[i][j]);
// 				lv_label_set_text(lv_obj_get_child(tiles[i][j], 0), "2048");
// 				lv_obj_center(lv_obj_get_child(tiles[i][j], 0));
// 				break;
// 			}
// 		}
// 	}
// }