1.如何添加自定义菜单按钮(蓝图)

+   参考: [UE5蓝图扩展编辑器菜单 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/641112033)
+   第一步, 为了方便找到菜单按钮插入口, 打开工程目录下的\Config\DefaultEditorPerProjectUserSettings.ini
+   添加以下内容:

```ini
[/Script/UnrealEd.EditorExperimentalSettings]
bEnableEditToolMenusUI=True
```

+   重启引擎, window->enable menu editing, 就可以在需要的地方看见小齿轮

![image-20240521161921777](E:\UE_Project\NewGame\plugins\Ray_EditorTool\Resources\DocImg\image-20240521161921777.png)

![image-20240521162013518](E:\UE_Project\NewGame\plugins\Ray_EditorTool\Resources\DocImg\image-20240521162013518.png)

![image-20240521162037699](E:\UE_Project\NewGame\plugins\Ray_EditorTool\Resources\DocImg\image-20240521162037699.png)

+   例如主界面点小齿轮或Edit Menu, 可以看到LevelEditor.MainMenu就是插入口

![image-20240521162119000](E:\UE_Project\NewGame\plugins\Ray_EditorTool\Resources\DocImg\image-20240521162119000.png)



+   新建蓝图类, 继承EditorUtilityToolMenuEntry
+   在class defaults编写data, menu可以是主插入口, 也可以找到一个sub插入口

![image-20240521162403130](E:\UE_Project\NewGame\plugins\Ray_EditorTool\Resources\DocImg\image-20240521162403130.png)

+   graph添加custom event, 命名为Run(固定规则命名)

![image-20240521162639448](E:\UE_Project\NewGame\plugins\Ray_EditorTool\Resources\DocImg\image-20240521162639448.png)

+   编写完graph后右键蓝图run可以看到效果
+   打开\Config\DefaultEditorPerProjectUserSettings.ini

```ini
[/Script/Blutility.EditorUtilitySubsystem]
StartupObjects=/Ray_EditorTool/Blueprint/Editor_Utilities/Menu/BP_EditorToolMenue.BP_EditorToolMenue
```

+   这个路径是右键蓝图copy reference中单引号的内容
+   重启引擎, 插入口出现自定义按钮
+   override function Event Execute可以编写按钮调用内容





![image-20240521164625384](E:\UE_Project\NewGame\plugins\Ray_EditorTool\Resources\DocImg\image-20240521164625384.png)

![image-20240521164643613](E:\UE_Project\NewGame\plugins\Ray_EditorTool\Resources\DocImg\image-20240521164643613.png)