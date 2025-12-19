param (
    [switch]$Configure,
    [switch]$Build,
    [switch]$Clean,
    [switch]$Help,
    [switch]$Verbose
)

# ==================================================
# CONFIGURATION
# ==================================================
$BuildDir  = "build"

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
    Write-Host "  .\build.ps1 -Configure -Build    " -NoNewline
    Write-Host "# Configure and build" -ForegroundColor Gray
    Write-Host "  .\build.ps1 -Clean               " -NoNewline
    Write-Host "# Clean build directory" -ForegroundColor Gray
    Write-Host "  .\build.ps1 -Verbose             " -NoNewline
    Write-Host "# Enable verbose output" -ForegroundColor Gray
    Write-Host "  .\build.ps1 -Help                " -NoNewline
    Write-Host "# Show this help" -ForegroundColor Gray
    Write-Host ""
    Write-Host "Examples:" -ForegroundColor Yellow
    Write-Host "  .\build.ps1 -Configure -Build -Verbose"
    Write-Host "  .\build.ps1 -Clean -Configure -Build"
    Write-Host ""
}

# ==================================================
# ARGUMENT VALIDATION
# ==================================================
$ValidArgsPassed = $Configure -or $Build -or $Clean -or $Help

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

Write-Host ""
Write-Host "All operations finished successfully." -ForegroundColor Green
Write-Host ""