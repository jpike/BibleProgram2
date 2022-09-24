#if 0

@ECHO OFF

REM MAKE SURE THE COMPILER IS CONFIGURED FOR x64.
REM This currently cannot be done via the C++ build system as just using std::system() to execute these batch files
REM does not result in appropriate environment variables being preserved.
IF NOT DEFINED VCINSTALLDIR (
    ECHO "Visual Studio tools not configured...Configuring for x64..."
    CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    REM TODO: Figure out why Visual Studio 2022 is returning 9009 instead of 0 here.
    IF %ERRORLEVEL% NEQ 9009 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    IF %ERRORLEVEL% NEQ 9009 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
    IF %ERRORLEVEL% NEQ 9009 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
)

IF "%VSCMD_ARG_TGT_ARCH%"=="x86" (
    ECHO "Incorrect Visual Studio target architecture...Reconfiguring for x64..."
    CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    REM TODO: Figure out why Visual Studio 2022 is returning 9009 instead of 0 here.
    IF %ERRORLEVEL% NEQ 9009 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    IF %ERRORLEVEL% NEQ 9009 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
    IF %ERRORLEVEL% NEQ 9009 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
)

ECHO "Double-checking for compiler..."
WHERE cl.exe
IF %ERRORLEVEL% NEQ 0 CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
IF %ERRORLEVEL% NEQ 0 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
IF %ERRORLEVEL% NEQ 0 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
IF %ERRORLEVEL% NEQ 0 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
WHERE cl.exe

REM BUILD THE C++ BUILD PROGRAM.
REM For exception handling flag - https://docs.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-160
REM /TP is needed to have this batch file treated as a .cpp file - https://docs.microsoft.com/en-us/cpp/build/reference/tc-tp-tc-tp-specify-source-file-type?view=msvc-160
cl.exe /I ../CppLibraries /std:c++latest /EHsc /TP build.bat

REM BUILD THE PROJECT
build.exe

COPY ..\CppLibraries\ThirdParty\SDL\SDL2.dll build\debug\SDL2.dll
COPY ..\CppLibraries\ThirdParty\SDL\SDL2.dll build\release\SDL2.dll

@ECHO ON
EXIT /B

#endif

#include <cstdlib>
#include <filesystem>
#include "BuildSystem/CppBuild.cpp"

int main()
{
    // DEFINE THE PATH TO THE WORKSPACE.
    // The path is converted to an absolute path to ensure it remains correct in all commands.
    std::filesystem::path workspace_folder_path = std::filesystem::absolute(".");

    // DEFINE THE BUILD TO ADD PROJECTS TO.
    Build build;

    // DEFINE PROJECTS FOR THE BUILD.
    Project windows_api = 
    {
        .Type = ProjectType::LIBRARY,
        .Name = "WindowsApi",
        .LinkerLibraryNames = { "user32.lib", "gdi32.lib" }
    };
    build.Add(&windows_api);

    Project direct_x = 
    {
        .Type = ProjectType::LIBRARY,
        .Name = "DirectX",
        .LinkerLibraryNames = { "d3d11.lib", "dxgi.lib", "d3dcompiler.lib" }
    };
    build.Add(&direct_x);

    Project open_gl = 
    {
        .Type = ProjectType::LIBRARY,
        .Name = "OpenGL",
        .LinkerLibraryNames = { "opengl32.lib" }
    };
    build.Add(&open_gl);

    Project cpp_libraries = 
    {
        .Type = ProjectType::LIBRARY,
        .Name = "CppLibraries",
        .AdditionalIncludeFolderPaths = 
        {
            workspace_folder_path / "../CppLibraries"
        },
        .AdditionalLibraryFolderPaths = 
        {
            workspace_folder_path / "../CppLibraries/build/debug",
            workspace_folder_path / "../CppLibraries/ThirdParty/SDL"
        },
        .Libraries = 
        {
            &windows_api,
            &direct_x,
            &open_gl,
        },
        .LinkerLibraryNames = 
        { 
            "gl3w.lib",
            "SDL2.lib", 
            "stmr.lib",
            "implot.lib",
            "CppLibraries.lib",
        },
    };
    build.Add(&cpp_libraries);

    Project bible_library = 
    {
        .Type = ProjectType::LIBRARY,
        .Name = "BibleLibrary",
        .CodeFolderPath = workspace_folder_path / "code",
        .UnityBuildFilepath = workspace_folder_path / "code/BibleLibrary/BibleLibrary.project",
        .AdditionalIncludeFolderPaths = 
        {
            workspace_folder_path / "../CppLibraries/ThirdParty",
            workspace_folder_path / "../CppLibraries/ThirdParty/imgui",
            workspace_folder_path / "../CppLibraries/ThirdParty/implot",
            workspace_folder_path / "../CppLibraries/ThirdParty/SDL",
            workspace_folder_path / "code/BibleLibrary",
        },
        .Libraries = 
        { 
            &cpp_libraries 
        },
        .LinkerLibraryNames = { "BibleLibrary.lib" },
    };
    build.Add(&bible_library);

    Project bible_data_generator = 
    {
        .Type = ProjectType::PROGRAM,
        .Name = "BibleDataGenerator",
        .CodeFolderPath = workspace_folder_path / "code",
        .UnityBuildFilepath = workspace_folder_path / "code/BibleDataGenerator/BibleDataGenerator.project",
        .Libraries = 
        { 
            &bible_library 
        }
    };
    build.Add(&bible_data_generator);

    Project bible_word_index_viewer = 
    {
        .Type = ProjectType::PROGRAM,
        .Name = "BibleWordIndexViewer",
        .CodeFolderPath = workspace_folder_path / "code",
        .UnityBuildFilepath = workspace_folder_path / "code/BibleWordIndexViewer/BibleWordIndexViewer.project",
        .Libraries = 
        { 
            &bible_library
        }
    };
    build.Add(&bible_word_index_viewer);

    Project bible_program_2 = 
    {
        .Type = ProjectType::PROGRAM,
        .Name = "BibleProgram2",
        .CodeFolderPath = workspace_folder_path / "code",
        .UnityBuildFilepath = workspace_folder_path / "code/BibleProgram2/BibleProgram2.project",
        .Libraries = 
        { 
            &bible_library 
        },
        .LinkerLibraryNames = 
        {
            "SDL2main.lib",
        }
    };
    build.Add(&bible_program_2);

    try
    {
        // BUILD DEBUG VERSIONS OF THE PROJECTS.
        int debug_build_exit_code = build.Run(workspace_folder_path, "debug");
        bool debug_build_succeeded = (EXIT_SUCCESS == debug_build_exit_code);
        if (!debug_build_succeeded)
        {
            return debug_build_exit_code;
        }

        // BUILD RELEASE VERSIONS OF THE PROJECT.
        /// @todo   Cleaner way to handle different build variants!
        cpp_libraries.AdditionalLibraryFolderPaths = 
        {
            workspace_folder_path / "../CppLibraries/build/release",
            workspace_folder_path / "../CppLibraries/ThirdParty/SDL"
        };
        int release_build_exit_code = build.Run(workspace_folder_path, "release");
        return release_build_exit_code;
    }
    catch (const std::exception& exception)
    {
        std::cout << "Standard exception: " << exception.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cout << "Unknown exception." << std::endl;
        return EXIT_FAILURE;
    }
}
