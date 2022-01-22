@ECHO OFF

REM MAKE SURE THE COMPILER IS CONFIGURED FOR x64.
IF NOT DEFINED VCINSTALLDIR (
    ECHO "Visual Studio tools not configured...Configuring for x64..."
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

IF "%VSCMD_ARG_TGT_ARCH%"=="x86" (
    ECHO "Incorrect Visual Studio target architecture...Reconfiguring for x64..."
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

ECHO "Double-checking for compiler..."
WHERE cl.exe
IF %ERRORLEVEL% NEQ 0 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
WHERE cl.exe

REM READ THE BUILD MODE COMMAND LINE ARGUMENT.
REM Either "debug" or "release" (no quotes).
REM If not specified, will default to debug.
IF NOT "%1" == "" (
    SET build_mode=%1
) ELSE (
    SET build_mode=debug
)

REM CREATE THE BUILD DIRECTORY.
SET build_directory=build\%build_mode%
IF NOT EXIST "%build_directory%" MKDIR "%build_directory%"
PUSHD "%build_directory%"

    REM DEFINE COMPILER OPTIONS.
    SET COMMON_COMPILER_OPTIONS=/EHsc /W4 /TP /std:c++latest /I ..\..\code /I ..\..\..\CppLibraries /I ..\..\..\CppLibraries\ThirdParty /I ..\..\..\CppLibraries\ThirdParty\SDL /I ..\..\code\BibleLibrary
    SET DEBUG_COMPILER_OPTIONS=%COMMON_COMPILER_OPTIONS% /Z7 /Od /MTd
    SET RELEASE_COMPILER_OPTIONS=%COMMON_COMPILER_OPTIONS% /O2 /MT
    SET LIBRARIES=SDL2.lib SDL2main.lib opengl32.lib gdi32.lib user32.lib

    REM BUILD AND RUN THE BIBLE DATA GENERATOR.
    SET BIBLE_DATA_GENERATOR_OUTPUT_FILE_BASE_NAME=BibleDataGenerator_%build_mode%
    SET bible_data_generator_compilation_command_line_arguments=/Fo:%BIBLE_DATA_GENERATOR_OUTPUT_FILE_BASE_NAME% ..\..\code\BibleDataGenerator\BibleDataGenerator.project
    ECHO %bible_data_generator_compilation_command_line_arguments%
    IF "%build_mode%"=="release" (
        cl.exe %RELEASE_COMPILER_OPTIONS% %bible_data_generator_compilation_command_line_arguments%
    ) ELSE (
        cl.exe %DEBUG_COMPILER_OPTIONS% %bible_data_generator_compilation_command_line_arguments%
    )

    IF %ERRORLEVEL% EQU 0 (
        %BIBLE_DATA_GENERATOR_OUTPUT_FILE_BASE_NAME%.exe
    )

    ECHO Done with %BIBLE_DATA_GENERATOR_OUTPUT_FILE_BASE_NAME% with %ERRORLEVEL%.

    REM BUILD THE BIBLE PROGRAM.
    SET BIBLE_PROGRAM_OUTPUT_FILE_BASE_NAME=BibleProgram2_%build_mode%
    SET bible_program_compilation_command_line_arguments=/Fo:%BIBLE_PROGRAM_OUTPUT_FILE_BASE_NAME% ..\..\code\BibleProgram2\BibleProgram2.project /link %LIBRARIES% /LIBPATH:..\..\..\CppLibraries\ThirdParty\SDL
    IF "%build_mode%"=="release" (
        cl.exe %RELEASE_COMPILER_OPTIONS% %bible_program_compilation_command_line_arguments%
    ) ELSE (
        cl.exe %DEBUG_COMPILER_OPTIONS% %bible_program_compilation_command_line_arguments%
    )

    COPY ..\..\..\CppLibraries\ThirdParty\SDL\SDL2.dll SDL2.dll

    ECHO Done building %BIBLE_PROGRAM_OUTPUT_FILE_BASE_NAME% with %ERRORLEVEL%.

    REM BUILD AND RUN THE BIBLE WORD INDEX VIEWER.
    SET BIBLE_WORD_INDEX_VIEWER_OUTPUT_FILE_BASE_NAME=BibleWordIndexViewer_%build_mode%
    SET bible_word_index_viewer_compilation_command_line_arguments=/Fo:%BIBLE_WORD_INDEX_VIEWER_OUTPUT_FILE_BASE_NAME% ..\..\code\BibleWordIndexViewer\BibleWordIndexViewer.project
    ECHO %bible_word_index_viewer_compilation_command_line_arguments%
    IF "%build_mode%"=="release" (
        cl.exe %RELEASE_COMPILER_OPTIONS% %bible_word_index_viewer_compilation_command_line_arguments%
    ) ELSE (
        cl.exe %DEBUG_COMPILER_OPTIONS% %bible_word_index_viewer_compilation_command_line_arguments%
    )

    IF %ERRORLEVEL% EQU 0 (
        %BIBLE_WORD_INDEX_VIEWER_OUTPUT_FILE_BASE_NAME%.exe
    )

    ECHO Done with %BIBLE_WORD_INDEX_VIEWER_OUTPUT_FILE_BASE_NAME% with %ERRORLEVEL%.

POPD

@ECHO ON
