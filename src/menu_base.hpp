#ifndef MENUBASE_HPP
#define MENUBASE_HPP

#include "session_io.hpp"
#include "common_io.hpp"
#include "session_data.hpp"
#include "ansi_processor.hpp"
#include "communicator.hpp"

#include "compat/menu_compat_dao.hpp"
#include "model/struct_compat.hpp"
#include "model/config.hpp"
#include "data/config_dao.hpp"
#include "mods/mod_base.hpp"

#include <functional>

class Config;
typedef boost::shared_ptr<Config> config_ptr;

class ConfigDao;
typedef boost::shared_ptr<ConfigDao> config_dao_ptr;

class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;

/**
 * @class MenuBase
 * @author Michael Griffin
 * @date 10/11/2016
 * @file menu_base.hpp
 * @brief Base Class for Menu System and Interfaces needing dynamic command execution
 */
class MenuBase
    : public MenuCompatDao
{
public:
    MenuBase(session_data_ptr session_data);
    ~MenuBase();
    
    session_data_ptr m_menu_session_data;
    
    // This hold non-hotkey text passed through.
    // If Hotkeys are turn off, we append and loop this until we hit a CRLF. or ENTER
    //session_data_ptr m_session_data;     // Handles to the session for i/o. {in TheState Base Class!}
    SessionIO        m_session_io;         // SessionIO for Output parsing and MCI Codes etc.
    CommonIO         m_common_io;          // CommonIO
    config_ptr       m_config;             // Config
    config_dao_ptr   m_config_dao;         // Config Data Access Object
    std::string      m_line_buffer;        // Buffer used for menu system and reading field data.
    bool             m_use_hotkey;         // Toggle for Single Hotkey or GetLine input. - Not used yet!
    std::string      m_current_menu;       // Name of current menu loaded.
    std::string      m_previous_menu;      // Name of Previous Menu for Gosub
    std::string      m_fallback_menu;      // Fallback, this can set as a Global Fall Back and changed via menu command
    int              m_input_index;        // Menu Input Index, for Forwarding to current function.

    // Temp
    MenuPrompt       m_menu_prompt;        // Hold the currently loaded menu prompt.
    MenuCompatInfo         m_menu_info;          // Menu Info
    MenuCompatOption       m_menu_options;       // Menu Commands

    ansi_process_ptr m_ansi_process;       // Instance for AnsiProcess Methods
    int              m_active_pulldownID;  // Active Lightbar Position.

    // Flags
    bool             m_fail_flag;                   // If menu or Option fails, kick off the fail flag.
    bool             m_pulldown_reentrace_flag;     // If menu or Option fails, kick off the fail flag.
    bool             m_is_active_pulldown_menu;     // If menu has active lightbars to display.


    // Holds all menu options/commands.
    std::vector<MenuCompatOption> m_loaded_menu_options;

    // Holds all menu prompts.
    std::vector<MenuPrompt> m_loaded_menu_prompts;

    // Holds all pulldown menu options.
    std::vector<MenuCompatOption> m_loaded_pulldown_options;

    // Dynamic Async Input Function Vector.
    std::vector<std::function< void(const std::string &, const bool &is_utf8)> > m_menu_functions;

    // Handle Dynamic modules being executed.
    std::vector<module_ptr> m_module;
    
    // Handles Dynamic Menu Command Option Execution
    std::vector<std::function< void(const MenuCompatOption &)> > m_execute_callback;

   /**
    * @brief Reads a Specific Menu, Info and Options { Called From readInMenuData() }
    */
    void clearMenuOptions();

   /**
    * @brief Clears out Loaded Pulldown options { Called From readInMenuData() }
    */
    void clearMenuPullDownOptions();


    /**
     * @brief Read Menu Data File for Info and all Options
     */
    void readInMenuData();
    void writeOutMenuData();
    void readMenuOptions();
    void writeMenuOptions();

    /**
     * @brief MenuSystemPrompts
     */
    void clearAllMenuPrompts();
    void readMenuAllPrompts();
    void readMenuPrompts(int menu_index);

    /**
     * @brief Loads or reloads and parses the ansi screen
     */
    std::string loadMenuScreen();

    /**
     * @brief Load the Menu into the system container.
     */
    void loadInMenu(std::string menu_name);

    /**
     * @brief Build string of lightbar matrices
     */
    std::string buildLightBars();

    /**
     * @brief De parse and display current menu system.
     */
    void redisplayMenuScreen();


    // Menu System will be a (2) Function system 1st setups up and displays
    // The Second handles all I/O for the menu options, this is dynamic since
    // Input is passed through to it, then the function returns for the next input
    // Since ere async, nothing stays inside it always returns.
    void startupMenu();

    /**
     * @brief Menu Editor, Runs through all existing menus
     */
    std::string displayMenus();

    /**
     * @brief Updates current and next lightbar positions.
     */
    void lightbarUpdate(int previous_pulldown_id);

    /**
     * @brief Handles Menu Option Call Back
     * @param input
     */
    void executeMenuOptions(const MenuCompatOption &option);

    void processMenuOptions(std::string &input);

    void menuInput(const std::string &character_buffer, const bool &is_utf8);
    
};

typedef boost::shared_ptr<MenuBase> menu_base_ptr;

#endif // MENUBASE_HPP
