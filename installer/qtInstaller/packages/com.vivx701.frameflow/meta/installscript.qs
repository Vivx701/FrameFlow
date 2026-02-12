function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {

        component.addOperation("CreateShortcut",
            "@TargetDir@/FrameFlow.exe",
            "@DesktopDir@/FrameFlow.lnk");

        component.addOperation("CreateShortcut",
            "@TargetDir@/FrameFlow.exe",
            "@StartMenuDir@/FrameFlow.lnk");
    }

    if (systemInfo.productType === "linux") {

        component.addOperation("CreateDesktopEntry",
            "@TargetDir@/frameflow.desktop",
            "Type=Application\n"
            + "Name=FrameFlow\n"
            + "Exec=@TargetDir@/FrameFlow\n"
            + "Icon=@TargetDir@/icon.png\n"
            + "Terminal=false\n"
            + "Categories=AudioVideo;");
    }
}
