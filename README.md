Cocos2d-x游戏
-----------
cocos2d-x 3.x引擎开发
跑酷类游戏和<抓住神经猫>的结合
借鉴了[The_b-dn][1]的3.x源码，参考了[u0u0][2]大神的Starter Kit，在此感谢

# 游戏现在基本完成，demo版可以正常使用，无明显bug，由于开发时间短，设计方面很渣，代码写的更渣，以后有时间准备**__重写一遍__**

主要bug：

 1. ~~小人跳起后落地不稳定，有可能翻转~~ 解决，刚体弹性设为0
 2. ~~小人吃金币有可能计数器多次++（由于使用了isVisible）~~ 解决（用isVisible判断..）
 3. ~~fatal，小人有可能被障碍物打击的翻转~~ 解决（setRotationEnable(false);）

To do:

 1. ~~开始页面StartLayer~~
 2. ~~结束页面EndLayer~~
 3. ~~跑酷和神经猫的连接~~
 4. ~~所有的button，记分牌等~~
 5. ~~除开始页面外的按键回调函数~~
 6. ~~暂停页面PauseLayer~~
 7. ~~点一下跳一下改为向上滑动跳一下~~
 8. ~~雾霾覆盖层，随着游戏进度增加雾霾减弱~~
 9. ~~开始界面图片和音乐部分~~
 10. ~~复杂的地形~~
 11. ~~抓住小雨滴页面的单点触控~~
 12. ~~所有资源整合到统一的plist里面~~
 13. ~~增加虫子，碰到虫子的话时间会减少~~
 
Some note:  

 1. 每次抓住了小雨滴之后雾霾透明度减少30
 2. 由电脑端切换到安卓端发布时，修改`#define onComputer false `，并且在appdelegate里面把windows模拟器大小那句去掉
 3. 虫子的tag是4

 

----------


> Written with [StackEdit](https://stackedit.io/).


  [1]: http://my.csdn.net/ZYY173533832
  [2]: http://www.tairan.com/archives/author/u0u0/
