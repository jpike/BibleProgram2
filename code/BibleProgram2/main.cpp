#include <cstdio>
#include <cstdlib>
#include <exception>
#include <optional>
#include <imgui/backends/imgui_impl_sdl.h>
#include <SDL/SDL.h>
#undef main
#include "Debugging/Timer.h"
#include "ErrorHandling/Asserts.h"
#include "Graphics/OpenGL/OpenGLGraphicsDevice.h"
#include "Windowing/SdlWindow.h"

#include "BibleData/Bibles.h"
#include "BibleData/BibleTranslation.h"
#include "BibleData/BibleWordIndex.h"
#include "BibleData/VerseCharacterRanges_KJV.h"
#include "BibleData/VerseCharacterRanges_WEB.h"
#include "BibleData/VerseText_KJV.h"
#include "BibleData/VerseText_WEB.h"
#include "Gui/Gui.h"
#include "Gui/UserSelections.h"
#include "Gui/UserSettings.h"

int main()
{
    std::printf("Starting program...\n");

    // CATCH ANY EXCEPTIONS.
    // A lot of things like SDL functions can easily fail.  To easily catch generic errors, everything's wrapped
    // in a try block, although it's not clear yet if that will sufficiently catch all errors.
    // If no errors occurred, success is assumed.
    int exit_code = EXIT_SUCCESS;
    // A few variables need to be exposed for shutdown.
    std::unique_ptr<WINDOWING::SdlWindow> window = nullptr;
    std::unique_ptr<GRAPHICS::OPEN_GL::OpenGLGraphicsDevice> graphics_device = nullptr;
    std::optional<GUI::Gui> gui = std::nullopt;
    try
    {
        // INITIALIZE BASIC DATA BASED ON BIBLE TRANSLATIONS.
        GUI::UserSettings user_settings;
        BIBLE_DATA::Bibles bibles;
        {
            std::printf("Initializing Bible translations...\n");
            DEBUGGING::SystemClockTimer system_clock_timer("Bible Data Initialization System Clock Timer");
            DEBUGGING::HighResolutionTimer high_resolution_timer("Bible Data Initialization High-Resolution Timer");
            BIBLE_DATA::BibleTranslation kjv_translation;
            kjv_translation.Text = BIBLE_DATA::KJV_VERSE_TEXT;
            kjv_translation.VerseCharacterRanges = BIBLE_DATA::KJV_VERSE_CHARACTER_RANGES;
            kjv_translation.WordIndex = BIBLE_DATA::BibleWordIndex::Build(&kjv_translation);
            bibles.TranslationsByAbbreviatedName[BIBLE_DATA::BibleTranslation::KJV_ABBREVIATED_NAME] = kjv_translation;
            user_settings.BibleTranslationDisplayStatusesByName[BIBLE_DATA::BibleTranslation::KJV_ABBREVIATED_NAME] = true;

            BIBLE_DATA::BibleTranslation web_translation;
            web_translation.Text = BIBLE_DATA::WEB_VERSE_TEXT;
            web_translation.VerseCharacterRanges = BIBLE_DATA::WEB_VERSE_CHARACTER_RANGES;
            web_translation.WordIndex = BIBLE_DATA::BibleWordIndex::Build(&web_translation);
            bibles.TranslationsByAbbreviatedName[BIBLE_DATA::BibleTranslation::WEB_ABBREVIATED_NAME] = web_translation;
            user_settings.BibleTranslationDisplayStatusesByName[BIBLE_DATA::BibleTranslation::WEB_ABBREVIATED_NAME] = false;
        }

        // INITIALIZE SDL.
        constexpr uint32_t SDL_SUBSYSTEMS = (SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO);
        constexpr int SDL_SUCCESS_RETURN_CODE = 0;
        int sdl_initialization_return_code = SDL_Init(SDL_SUBSYSTEMS);
        bool sdl_initialized = (SDL_SUCCESS_RETURN_CODE == sdl_initialization_return_code);
        ASSERT_THEN_IF_NOT(sdl_initialized)
        {
            std::fprintf(stderr, "SDL_Init() failed.  Error: %s\n", SDL_GetError());
            return EXIT_FAILURE;
        }

        // CREATE THE WINDOW.
        // It is maximized to allow most effective use of screenspace.
        constexpr int SCREEN_WIDTH_IN_PIXELS = 1400;
        constexpr int SCREEN_HEIGHT_IN_PIXELS = 900;
        window = WINDOWING::SdlWindow::Create(
            "Bible Program 2", 
            SCREEN_WIDTH_IN_PIXELS, 
            SCREEN_HEIGHT_IN_PIXELS, 
            GRAPHICS::HARDWARE::GraphicsDeviceType::OPEN_GL,
            SDL_WINDOW_MAXIMIZED);
        ASSERT_THEN_IF_NOT(window)
        {
            std::fprintf(stderr, "Failed to create window.  SDL Error: %s\n", SDL_GetError());
            SDL_Quit();
            return EXIT_FAILURE;
        }

        // INITIALIZE THE GRAPHICS DEVICE.
        graphics_device = GRAPHICS::OPEN_GL::OpenGLGraphicsDevice::ConnectTo(*window);

        // CREATE THE GUI.
        gui = GUI::Gui::Create(*window, *graphics_device);

        // CREATE THE INITIAL USER SELECTIONS.
        GUI::UserSelections user_selections;

        // UPDATE AND DISPLAY THE WINDOW WHILE IT REMAINS OPEN.
        while (window->IsOpen)
        {
            // HANDLE APPLICATION EVENTS.
            SDL_Event application_event;
            while (SDL_PollEvent(&application_event) != 0)
            {
                ImGui_ImplSDL2_ProcessEvent(&application_event);
                if (SDL_QUIT == application_event.type)
                {
                    window->IsOpen = false;
                }
            }

            // UPDATE AND RENDER THE GUI.
            graphics_device->ClearBackground(GRAPHICS::Color::BLACK);

            gui->UpdateAndRender(*window, bibles, user_selections, user_settings);

            graphics_device->DisplayRenderedImage(*window);
        }
    }
    catch (const std::exception& exception)
    {
        std::fprintf(stderr, "Standard exception: %s\nSDL error: %s\n", exception.what(), SDL_GetError());
        exit_code = EXIT_FAILURE;
    }
    catch (...)
    {
        std::fprintf(stderr, "Unknown exception.\nSDL error: %s\n", SDL_GetError());
        exit_code = EXIT_FAILURE;
    }

    // SHUTDOWN SUBSYSTEMS.
    if (gui)
    {
        gui->Shutdown();
    }
    if (graphics_device)
    {
        graphics_device->Shutdown();
    }
    if (window)
    {
        window->Close();
    }
    SDL_Quit();

    std::printf("Ending program...\n");
    return exit_code;
}
