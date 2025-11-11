## Introduction
1. Based on lvgl8.3 graphics library,use the canvas and rectangular text objects to implement a 4x4 grid space.Through the 2048 game's logic,regularly refresh the numbers such as 0,2 and 4represented on the grids.  
基于lvgl8.3图形库，用画布和矩形文本对象实现4*4格子空间，通过2048游戏本体逻辑，定期刷新格子上代表的0，2，4等等。  
2. The game first initialization the styles of all tiles,and applies the "style_tile_0" style with the number 0.Regular refreshes will change the tile styles according to the maintained array(matrix) when different numbers are present(such as "style_tile_2").  
游戏先初始化所有格子的样式，并带上数字0（style_tile_0）样式，定期刷新会根据维护的数组(matrix)来更换不同数字时格子样式（style_tile_2）

## 辅助资源
不同颜色的24位编码 https://vuetifyjs.com/en/styles/colors/#material-colors
