param (
    [switch]$Configure,
    [switch]$Build,
    [switch]$Deploy,
    [switch]$Clean,
    [switch]$Help,
    [switch]$Verbose
)

# ==================================================
# CONFIGURATION
# ==================================================
$BuildDir  = "build"
$DeployDir = "$BuildDir\deploy"

# Qt (MinGW build)
$QtPath    = "C:\Qt\6.8.3\mingw_64"

# MinGW toolchain
$MinGWRoot = "C:\Qt\Tools\mingw1310_64"
$MinGWBin  = "$MinGWRoot\bin"

$Jobs = [Environment]::ProcessorCount

# ==================================================
# HELP
# ==================================================
function Show-Help {
    Write-Host ""
    Write-Host "FrameFlow build script (Windows / Qt MinGW)" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Usage:" -ForegroundColor Yellow
    Write-Host "  .\build.ps1 -Configure           " -NoNewline
    Write-Host "# Configure the project" -ForegroundColor Gray
    Write-Host "  .\build.ps1 -Build               " -NoNewline
    Write-Host "# Build the project" -ForegroundColor Gray
    Write-Host "  .\build.ps1 -Deploy              " -NoNewline
    Write-Host "# Package app with Qt dependencies" -ForegroundColor Gray
    Write-Host "  .\build.ps1 -Configure -Build    " -NoNewline
    Write-Host "# Configure and build" -ForegroundColor Gray
    Write-Host "  .\build.ps1 -Build -Deploy       " -NoNewline
    Write-Host "# Build and package" -ForegroundColor Gray
    Write-Host "  .\build.ps1 -Clean               " -NoNewline
    Write-Host "# Clean build directory" -ForegroundColor Gray
    Write-Host "  .\build.ps1 -Verbose             " -NoNewline
    Write-Host "# Enable verbose output" -ForegroundColor Gray
    Write-Host "  .\build.ps1 -Help                " -NoNewline
    Write-Host "# Show this help" -ForegroundColor Gray
    Write-Host ""
    Write-Host "Examples:" -ForegroundColor Yellow
    Write-Host "  .\build.ps1 -Configure -Build -Verbose"
    Write-Host "  .\build.ps1 -Clean -Configure -Build -Deploy"
    Write-Host "  .\build.ps1 -Deploy"
    Write-Host ""
}

# ==================================================
# ARGUMENT VALIDATION
# ==================================================
$ValidArgsPassed = $Configure -or $Build -or $Deploy -or $Clean -or $Help

if (-not $ValidArgsPassed) {
    Write-Host ""
    Write-Error "ERROR: No valid arguments provided!"
    Show-Help
    exit 1
}

if ($Help) {
    Show-Help
    exit 0
}

# ==================================================
# SAFETY CHECKS
# ==================================================
if (-not (Test-Path $QtPath)) {
    Write-Error "Qt not found at: $QtPath"
    exit 1
}

if (-not (Test-Path "$MinGWBin\g++.exe")) {
    Write-Error "MinGW compiler not found at: $MinGWBin"
    exit 1
}

# ==================================================
# CLEAN
# ==================================================
if ($Clean) {
    if (Test-Path $BuildDir) {
        Write-Host "Cleaning build directory..." -ForegroundColor Yellow
        Remove-Item -Recurse -Force $BuildDir
        Write-Host "Build directory cleaned." -ForegroundColor Green
    } else {
        Write-Host "Build directory does not exist, nothing to clean." -ForegroundColor Gray
    }

    if (Test-Path $DeployDir) {
        Write-Host "Cleaning deploy directory..." -ForegroundColor Yellow
        Remove-Item -Recurse -Force $DeployDir
        Write-Host "Deploy directory cleaned." -ForegroundColor Green
    }
}

# ==================================================
# CONFIGURE
# ==================================================
if ($Configure) {

    Write-Host ""
    Write-Host "Configuring FrameFlow..." -ForegroundColor Cyan
    Write-Host ""

    if (-not (Test-Path $BuildDir)) {
        New-Item -ItemType Directory $BuildDir | Out-Null
    }

    # ------------------------------------------------
    # IMPORTANT:
    # Convert all paths to forward slashes for CMake
    # ------------------------------------------------
    $QtPathCMake   = $QtPath   -replace '\\', '/'
    $MinGWBinCMake = $MinGWBin -replace '\\', '/'

    # Inject MinGW into PATH (session only)
    $env:PATH = "$MinGWBin;$env:PATH"

    # Build CMake arguments
    $CMakeArgs = @(
        "-S", ".",
        "-B", $BuildDir,
        "-G", "MinGW Makefiles",
        "-DCMAKE_BUILD_TYPE=Release",
        "-DCMAKE_PREFIX_PATH=$QtPathCMake",
        "-DCMAKE_CXX_COMPILER=$MinGWBinCMake/g++.exe"
    )

    # Add verbose flag if requested
    if ($Verbose) {
        $CMakeArgs += "--debug-output"
        $CMakeArgs += "--trace-expand"
        Write-Host "Verbose mode enabled" -ForegroundColor Yellow
        Write-Host ""
    }

    # Execute CMake
    & cmake @CMakeArgs

    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Error "CMake configuration failed"
        exit 1
    }

    Write-Host ""
    Write-Host "Configuration completed successfully." -ForegroundColor Green
}

# ==================================================
# BUILD
# ==================================================
if ($Build) {

    Write-Host ""
    Write-Host "Building FrameFlow (and FFmpeg if needed)..." -ForegroundColor Cyan
    Write-Host ""

    # Build CMake build arguments
    $BuildArgs = @(
        "--build", $BuildDir,
        "--parallel", $Jobs
    )

    # Add verbose flag if requested
    if ($Verbose) {
        $BuildArgs += "--verbose"
        Write-Host "Verbose build mode enabled" -ForegroundColor Yellow
        Write-Host ""
    }

    # Execute build
    & cmake @BuildArgs

    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Error "Build failed"
        exit 1
    }

    Write-Host ""
    Write-Host "Build completed successfully." -ForegroundColor Green
}

# ==================================================
# DEPLOY
# ==================================================
if ($Deploy) {

    Write-Host ""
    Write-Host "Deploying FrameFlow..." -ForegroundColor Cyan
    Write-Host ""

    # Check if executable exists
    $ExePath = "$BuildDir\app\FrameFlow.exe"
    if (-not (Test-Path $ExePath)) {
        Write-Error "Executable not found at: $ExePath"
        Write-Host "Please build the project first with -Build" -ForegroundColor Yellow
        exit 1
    }

    # Create deploy directory
    if (Test-Path $DeployDir) {
        Write-Host "Cleaning existing deploy directory..." -ForegroundColor Yellow
        Remove-Item -Recurse -Force $DeployDir
    }
    New-Item -ItemType Directory $DeployDir | Out-Null

    Write-Host "Copying application..." -ForegroundColor Yellow

    # Copy main executable
    Copy-Item $ExePath $DeployDir

    # Copy frameflow-writer DLL
    # libframeflow-writer.dll
    $WriterDLL = "$BuildDir\frameflow-writer\libframeflow-writer.dll"
    if (Test-Path $WriterDLL) {
        Copy-Item $WriterDLL $DeployDir
        Write-Host "  [OK] libframeflow-writer.dll" -ForegroundColor Green
    }

    # Copy FFmpeg DLLs
    Write-Host "Copying FFmpeg libraries..." -ForegroundColor Yellow
    $FFmpegBinDir = "third_party\ffmpeg\bin"
    if (Test-Path $FFmpegBinDir) {
        # Find the actual bin directory (might be in a subdirectory)
        $FFmpegDLLs = Get-ChildItem -Path "third_party\ffmpeg" -Recurse -Filter "*.dll" -File
        foreach ($dll in $FFmpegDLLs) {
            Copy-Item $dll.FullName $DeployDir
            Write-Host "  [OK] $($dll.Name)" -ForegroundColor Green
        }
    } else {
        Write-Warning "FFmpeg DLLs not found in third_party\ffmpeg"
    }

    # Run windeployqt
    Write-Host "Running windeployqt..." -ForegroundColor Yellow
    $WinDeployQt = "$QtPath\bin\windeployqt.exe"

    if (-not (Test-Path $WinDeployQt)) {
        Write-Error "windeployqt not found at: $WinDeployQt"
        exit 1
    }

    $WinDeployArgs = @(
        "$DeployDir\FrameFlow.exe",
        "--release",
        "--no-compiler-runtime"
    )

    if ($Verbose) {
        $WinDeployArgs += "--verbose", "2"
    }

    & $WinDeployQt @WinDeployArgs

    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Error "windeployqt failed"
        exit 1
    }

    # Copy MinGW runtime DLLs
    Write-Host "Copying MinGW runtime libraries..." -ForegroundColor Yellow
    $MinGWDLLs = @(
        "libgcc_s_seh-1.dll",
        "libstdc++-6.dll",
        "libwinpthread-1.dll"
    )

    foreach ($dll in $MinGWDLLs) {
        $dllPath = "$MinGWBin\$dll"
        if (Test-Path $dllPath) {
            Copy-Item $dllPath $DeployDir
            Write-Host "  [OK] $dll" -ForegroundColor Green
        } else {
            Write-Warning "MinGW DLL not found: $dll"
        }
    }

    Write-Host ""
    Write-Host "Deployment completed successfully!" -ForegroundColor Green
    Write-Host "Deployed application location: $DeployDir" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "You can now run:" -ForegroundColor Yellow
    Write-Host "  .\$DeployDir\FrameFlow.exe" -ForegroundColor White
}

Write-Host ""
Write-Host "All operations finished successfully." -ForegroundColor Green
Write-Host ""
