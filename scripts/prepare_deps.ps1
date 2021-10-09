$PROJECT_ROOT = "$PSScriptRoot\.."
$OF_RELEASE = "of_v0.11.2_vs2017_release"
$OF_DEP_URL = "https://s3.us-west-1.amazonaws.com/photos.urbn.photos/deps/$OF_RELEASE.zip"
$OF_ZIP_PATH = "$PROJECT_ROOT\deps\of.zip"
$OF_DEP_PATH = "$PROJECT_ROOT\deps\of\of"

function OpenFramework {

	# If we don't already have the zip file present, download it
	if (-not(Test-Path -Path $OF_ZIP_PATH -PathType Leaf)) {
		Write-Host "Downloading OpenFrameworks Release"
		Invoke-WebRequest -Uri $OF_DEP_URL -OutFile $OF_ZIP_PATH
	}

	# If We haven't unzipped and renamed the path, let's do it.
	if (-not(Test-Path -Path $OF_DEP_PATH -PathType Container)) {
		Write-Host "Unzipping OpenFrameworks Release"
		$ZIP_DEST = Split-Path -Path $OF_DEP_PATH -Parent
		Expand-Archive -LiteralPath $OF_ZIP_PATH -DestinationPath $ZIP_DEST
		Move-Item -Path "$PROJECT_ROOT\deps\of\$OF_RELEASE\" -Destination $OF_DEP_PATH
	}

	Write-Host "Patching OpenFrameworks for VS 2019"
	$PatchPath =  "$PROJECT_ROOT\patches\openframeworks\"
	$Files = Get-ChildItem -Path $PatchPath
	$DestPath = "$OF_DEP_PATH\libs\openFrameworksCompiled\project\vs\"
	foreach($f in $Files) {
		Copy-Item -Path "$PatchPath\$f" -Destination $DestPath
	}
}


OpenFramework
## TODO: Install addons by downloading or checking out.
