# 文明4 刀剑之上 XML Load Error 修复程序 v0.1

本程序用于修复在中文系统上运行 文明4 刀剑之上 时出现的 XML Load Error。

（把操作系统语言改成英文也能解决问题，但不优雅）

## 作者
Tom Willow

## 下载地址

[文明4 刀剑之上 XML Load Error 修复程序 v0.1](https://github.com/tomwillow/Repair-Civ4-BTS-XML-Load-Error/releases/download/v0.1/Repair.Civ4.BTS.XML.Load.Error.v0.1.zip)

## 原理

游戏内有些XML文件带有欧洲语言的特殊字符，例如德语的ü。

在中文系统下，这些特殊字符由于ASCII码超过127，会和后面的</German>等标签粘连识别为中文，导致</开头的结束标签不能正确识别。

本程序运行时将扫描所在位置处所有XML文件，在发生粘连的位置加空格，以解决此问题。

## 使用方法

将本程序放入游戏文件夹（例如 Sid Meier's Civilization IV Beyond the Sword），然后运行。

## 效果图

![](https://github.com/tomwillow/Repair-Civ4-BTS-XML-Load-Error/blob/master/build/snap.PNG)

## 参考

 - [\[原创\] 文明4解决XML文件乱码的终极办法。(附上转换程序）](http://www.civclub.net/bbs/forum.php?mod=viewthread&tid=33847)
``
（我才不会告诉你是因为我在这个网站一直收不到验证邮件下载不了附件才自己写了一个程序）
``
 - [https://github.com/yhlleo/FindFilesWithinFolder](https://github.com/yhlleo/FindFilesWithinFolder)