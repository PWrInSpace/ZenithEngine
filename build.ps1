param($command, $mode)

function Display-Info
{
    Write-Output "`n>Zenith Engine CLI build tool syntax:"
    Write-Output "    build.ps1"
    Write-Output "        <COMMAND: (default - build project)"
    Write-Output "            help - display tool syntax (MODE not required)"
    Write-Output "            init - initialize submodules (MODE not required)"
    Write-Output "            up - update submodules (MODE not required)"
    Write-Output "            gen - generate build system"
    Write-Output "            run - run tech demo>"
    Write-Output "        <MODE: R|Release; Ri|ReleaseInfo; D|Debug; CI|Static analysis setup>`n"
}

Switch ($command)
{
    "init"
    {
        git submodule update --init
        exit 0
    }
    "up"
    {
        git submodule update --remote --merge
        exit 0
    }
    "help"
    {
        Display-Info
        exit 0
    }
    default
    {
        if (!$mode)
        {
            $mode=$command
        }
    }
}

$build_type
Switch ($mode.ToLower())
{
    {($_ -eq "r") -or ($_ -eq "release") -or ($_ -eq "ci")}
    {
        $build_type="Release"
        break
    }
    {($_ -eq "ri") -or ($_ -eq "releaseinfo")}
    {
        $build_type="RelWithDebInfo"
        break
    }
    {($_ -eq "d") -or ($_ -eq "debug")}
    {
        $build_type="Debug"
        break
    }
    default
    {
        Write-Output "`nNot specified correct build mode!"
        Display-Info
        exit -1
    }
}

$obj_dir="Build/$build_type"
$bin_dir="Bin/$build_type"
Switch ($command)
{
    "gen"
    {
        $args=""
        if ($mode -eq "CI")
        {
            $args="-DZE_CI_JOB=ON"
        }
        cmake $args -D CMAKE_BUILD_TYPE=$build_type -G Ninja -S . -B "$obj_dir"
        break
    }
    "run"
    {
        if (Test-Path -Path "$bin_dir/ZenithDemo.exe" -PathType Leaf)
        {
            cd "$bin_dir"
            Start-Process -NoNewWindow "./ZenithDemo.exe"
            cd ../..
        }
        else
        {
            Write-Output "`nApplication must be built first!`n"
        }
        break
    }
    default
    {
        if (Test-Path -Path "$obj_dir" -PathType Container)
        {
            cmake --build "$obj_dir" --config $build_type
        }
        else
        {
            Write-Output "`nBuild system must be generated first!`n"
        }
        break
    }
}