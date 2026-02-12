param (
    [string]$OutputName = "FrameFlow-Installer.exe"
)

$ConfigPath = "config/config.xml"
$PackagesPath = "packages"

Write-Host "====================================="
Write-Host " Building FrameFlow Installer (QtIFW)"
Write-Host "====================================="

if (!(Test-Path $ConfigPath)) {
    Write-Host "Config file not found!"
    exit 1
}

if (!(Test-Path $PackagesPath)) {
    Write-Host "Packages folder not found!"
    exit 1
}

# Make sure binarycreator is in PATH or give full path
binarycreator.exe --config $ConfigPath --packages $PackagesPath $OutputName

if ($LASTEXITCODE -eq 0) {
    Write-Host "Installer created successfully: $OutputName"
} else {
    Write-Host "Installer build failed!"
    exit 1
}
