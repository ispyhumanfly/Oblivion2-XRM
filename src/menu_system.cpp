#include "menu_system.hpp"

#include "data/config_dao.hpp"
#include "mods/mod_prelogon.hpp"
#include "mods/mod_logon.hpp"
#include "mods/mod_signup.hpp"

#include <string>
#include <vector>
#include <functional>


const std::string MenuSystem::m_menuID = "MENU_SYSTEM";

MenuSystem::MenuSystem(session_data_ptr session_data)
    : StateBase(session_data)
    , MenuBase(session_data)
{
    std::cout << "MenuSystem" << std::endl;

    // [Vector] Setup std::function array with available options to pass input to.
    m_menu_functions.push_back(std::bind(&MenuBase::menuInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuSystem::menuEditorInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuSystem::modulePreLogonInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuSystem::moduleLogonInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuSystem::moduleInput, this, std::placeholders::_1, std::placeholders::_2));

    // [Vector] Setup Menu Option Calls for executing menu commands.
    m_execute_callback.push_back(std::bind(&MenuSystem::menuOptionsCallback, this, std::placeholders::_1));
            
    // [Mapped] Setup Menu Command Functions    
    m_menu_command_functions['-'] = std::bind(&MenuSystem::menuOptionsControlCommands, this, std::placeholders::_1);
    m_menu_command_functions['&'] = std::bind(&MenuSystem::menuOptionsMultiNodeCommands, this, std::placeholders::_1);
    m_menu_command_functions['{'] = std::bind(&MenuSystem::menuOptionsMatrixCommands, this, std::placeholders::_1);
    m_menu_command_functions['!'] = std::bind(&MenuSystem::menuOptionsGlobalNewScanCommands, this, std::placeholders::_1);
    m_menu_command_functions['['] = std::bind(&MenuSystem::menuOptionsMainMenuCommands, this, std::placeholders::_1);
    m_menu_command_functions['.'] = std::bind(&MenuSystem::menuOptionsDoorCommands, this, std::placeholders::_1);
    m_menu_command_functions['*'] = std::bind(&MenuSystem::menuOptionsSysopCommands, this, std::placeholders::_1);
    m_menu_command_functions['^'] = std::bind(&MenuSystem::menuOptionsNewUserVotingCommands, this, std::placeholders::_1);
    m_menu_command_functions['C'] = std::bind(&MenuSystem::menuOptionsConferenceEditorCommands, this, std::placeholders::_1);
    m_menu_command_functions['D'] = std::bind(&MenuSystem::menuOptionsDataAreaCommands, this, std::placeholders::_1);
    m_menu_command_functions['E'] = std::bind(&MenuSystem::menuOptionsEmailCommands, this, std::placeholders::_1);
    m_menu_command_functions['F'] = std::bind(&MenuSystem::menuOptionsFileCommands, this, std::placeholders::_1);
    m_menu_command_functions['J'] = std::bind(&MenuSystem::menuOptionsJoinConference, this, std::placeholders::_1);
    m_menu_command_functions['M'] = std::bind(&MenuSystem::menuOptionsMessageCommands, this, std::placeholders::_1);
    m_menu_command_functions['Q'] = std::bind(&MenuSystem::menuOptionsQWKMailCommands, this, std::placeholders::_1);
    m_menu_command_functions['R'] = std::bind(&MenuSystem::menuOptionsTopTenListingCommands, this, std::placeholders::_1);
    m_menu_command_functions['S'] = std::bind(&MenuSystem::menuOptionsMessageBaseSponsorCommands, this, std::placeholders::_1);
    m_menu_command_functions['T'] = std::bind(&MenuSystem::menuOptionsFileBaseSponsorCommands, this, std::placeholders::_1);
    m_menu_command_functions['V'] = std::bind(&MenuSystem::menuOptionsVotingCommands, this, std::placeholders::_1);
    m_menu_command_functions['+'] = std::bind(&MenuSystem::menuOptionsColorSettingCommands, this, std::placeholders::_1);
    

    // Load the configuration file to the class
    if (!m_config_dao->loadConfig())
    {
        std::cout << "Error: unable to load configuration file" << std::endl;
        assert(false);
    }
}

MenuSystem::~MenuSystem()
{
    std::cout << "~MenuSystem" << std::endl;
    
    // Clear All Menu Command Functions.
    MappedCommandFunctions().swap(m_menu_command_functions);
}

/**
 * @brief Handles Updates or Data Input from Client
 */
void MenuSystem::update(const std::string &character_buffer, const bool &is_utf8)
{
    if(!m_is_active)
    {
        return;
    }

    // This simplily passed through the input to the current system fuction were at.
    m_menu_functions[m_input_index](character_buffer, is_utf8);
}

/**
 * @brief Startup class, setup initial screens / interface, flags etc..
 *        This is only called when switch to the state, not for menu instances.
 * @return
 */
bool MenuSystem::onEnter()
{
    std::cout << "OnEnter() MenuSystem" << std::endl;

    // Startup the Prelogon sequence
    startupModulePreLogon();
    m_is_active = true;
    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool MenuSystem::onExit()
{
    std::cout << "OnExit() MenuSystem\n";
    m_is_active = false;
    return true;
}


/**
 * TODO - Menu Options, lets break out each section
 * option.command_key[1] into their own methods
 * Making this call back smaller.
 */
 
 
/**
 * @brief Control Commands
 * @param option
 */
bool MenuSystem::menuOptionsControlCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
            // Turns on Pulldown Menu Re-entrance
            // This option returns to the selected option
            // when the user re-enters the pulldown menu.
            // This works ONLY if the command that the user
            // executed does not go to another menu.
        case '\'':
            return false;
            // Turns off Pulldown Menu Re-Entrance
            
        case '`':
            return false;
            
        default:
            return false;
    }

    return true;
}

/**
 * @brief MultiNode Commands
 * @param option
 */
bool MenuSystem::menuOptionsMultiNodeCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Matrix Commands
 * @param option
 */
bool MenuSystem::menuOptionsMatrixCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
            // Logon
            // {Not Implimented yet!}
            //: When a CString is specified, PRELOGON.X, an
            //: USERLOG.X, and SYSPASS.X will be displayed.
            // { Note: add 0 for random! }
        case 'S':
            std::cout << "Executing startupModuleLogon()" << std::endl;
            startupModuleLogon();
            break;
            
            // Command Key: {T  {Research more how this is used!}
            // Function   : Sets FailFlag to false if user is able to get to
            //     : main system through knowing the system password
            //     : and having an account.
            //     : This would be used for stacking commands at the
            //     : matrix without actually logging onto to the
            //     : board.
        case 'T':
            return false;

            // Apply
        case 'A':
            std::cout << "Executing startupModuleSignup();" << std::endl;
            startupModuleSignup();
            return true;

            // Check
        case 'C':
            return false;

            // Feedback
        case 'F':
            return false;

            // Chat
        case 'P':
            return false;

            // Logoff
        case 'G':
            std::cout << "Goodbye;" << std::endl;
            m_session_data->logoff();
            break;
                       
            // Drops into the BBS
        case 'X':
            return false;
            
        default:
            return false;
    }

    return true;
}

/**
 * @brief Global New Scan Commands
 * @param option
 */
bool MenuSystem::menuOptionsGlobalNewScanCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Main Menu Commands
 * @param option
 */
bool MenuSystem::menuOptionsMainMenuCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Door Commands
 * @param option
 */
bool MenuSystem::menuOptionsDoorCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Sysop Commands
 * @param option
 */
bool MenuSystem::menuOptionsSysopCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
            // Menu Editor
        case '#':
            return false;

            // Configuration Menu
        case 'C':
            return false;
            
        default:
            return false;
    }

    return true;
}

/**
 * @brief New User Voting Commands
 * @param option
 */
bool MenuSystem::menuOptionsNewUserVotingCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Conference Editor Commands
 * @param option
 */
bool MenuSystem::menuOptionsConferenceEditorCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Data Area Commands
 * @param option
 */
bool MenuSystem::menuOptionsDataAreaCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Email Commands
 * @param option
 */
bool MenuSystem::menuOptionsEmailCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief File Commands
 * @param option
 */
bool MenuSystem::menuOptionsFileCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Message Commands
 * @param option
 */
bool MenuSystem::menuOptionsMessageCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Join Conference Commands
 * @param option
 */
bool MenuSystem::menuOptionsJoinConference(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief QWK Mail Commands
 * @param option
 */
bool MenuSystem::menuOptionsQWKMailCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Top 10 Listing Commands
 * @param option
 */
bool MenuSystem::menuOptionsTopTenListingCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Message Base Sponsor Commands
 * @param option
 */
bool MenuSystem::menuOptionsMessageBaseSponsorCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief File Base Sponsor Commands
 * @param option
 */
bool MenuSystem::menuOptionsFileBaseSponsorCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Voting Commands
 * @param option
 */
bool MenuSystem::menuOptionsVotingCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Color Setting Commands
 * @param option
 */
bool MenuSystem::menuOptionsColorSettingCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Process Command Keys passed from menu selection (Callback)
 * @param option
 */
bool MenuSystem::menuOptionsCallback(const MenuOption &option)
{
    /* Run through the case and switch over the new interface.
    std::string mnuOption = option.CKeys;
    std::string mnuString = option.CString;
    std::string mnuAccess = option.Acs; */

    // If Invalid then return
    if(option.command_key.size() != 2)
    {
        return false;
    }
     
    std::string firstCommandKeyIndex = "-&{![.*^CDEFJMQRSTV+";
    std::string::size_type idx = 0;

    // If valid then execute the related Menu Command Function
    idx = firstCommandKeyIndex.find(option.command_key[0], 0);
    if (idx != std::string::npos) 
    {
        return m_menu_command_functions[option.command_key[0]](option);
    }
           
    return false;
}

/**
 * @brief Resets the Menu Input Method in the Function Array
 * @param index
 */
void MenuSystem::resetMenuInputIndex(int index)
{
    m_input_index = index;
}

/**
 * @brief Startup Menu Editor and Switch to MenuEditorInput
 */
void MenuSystem::startupMenuEditor()
{
    std::cout << "Entering MenuEditor Input " << std::endl;

    // Setup the input processor
    resetMenuInputIndex(MENU_EDITOR_INPUT);

    // 2. Handle any init and startup Screen Displays

    // WIP, nothing completed just yet for the startup.
}

/**
 * @brief Menu Editor, Read and Modify Menus
 */
void MenuSystem::menuEditorInput(const std::string &character_buffer, const bool &)
{
    // If were not using hot keys, loop input until we get ENTER
    // Then handle only the first key in the buffer.  Other "SYSTEMS"
    // Will parse for entire line for matching Command Keys.
    if (!m_use_hotkey)
    {
        // Received ENTER, grab the previous input.
        if (!(character_buffer[0] == 13))
        {
            m_line_buffer += character_buffer[0];
            m_session_data->deliver(m_line_buffer);
        }
    }
    else
    {
        m_line_buffer += character_buffer[0];
    }

    std::string output_buffer = "";
    switch (std::toupper(m_line_buffer[0]))
    {
        case 'A': // Add
            output_buffer = "Enter Menu Name to Add : ";
            break;
        case 'C': // Change/Edit
            output_buffer = "Enter Menu Name to Change : ";
            break;
        case 'D': // Delete
            output_buffer = "Enter Menu to Delete : ";
            break;
        case 'Q': // Quit
            // Reload fall back, or gosub to last menu!

            return;
        default : // Return
            return;
    }

    m_session_data->deliver(output_buffer);
}

/**
 * @brief Clears All Modules
 */
void MenuSystem::clearAllModules()
{
    if (m_module.size() > 0)
    {
        std::vector<module_ptr>().swap(m_module);
    }
}

/**
 * @brief Exists and Shutsdown the current module
 */
void MenuSystem::shutdownModule()
{
    // Do module shutdown, only single modules are loaded
    // This makes it easy to allocate and kill on demand.
    m_module[0]->onExit();
    m_module.pop_back();
}

/**
 * @brief Exists and Shutsdown the current module
 */
void MenuSystem::startupModule(module_ptr module)
{
    // First clear any left overs if they exist.
    clearAllModules();

    // Run the startup for the module
    module->onEnter();

    // Push to stack now the new module.
    m_module.push_back(module);
}

/**
 * @brief Start up the Normal Login Process.
 */
void MenuSystem::startupModulePreLogon()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_PRELOGON_INPUT);

    // Allocate and Create
    module_ptr module(new ModPreLogon(m_session_data, m_config, m_ansi_process));
    if (!module)
    {
        std::cout << "ModPreLogon Allocation Error!" << std::endl;
        assert(false);
    }

    startupModule(module);
}

/**
 * @brief Start up the Normal Login Process.
 */
void MenuSystem::startupModuleLogon()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_LOGON_INPUT);

    // Allocate and Create
    module_ptr module(new ModLogon(m_session_data, m_config, m_ansi_process));
    if (!module)
    {
        std::cout << "ModLogon Allocation Error!" << std::endl;
        assert(false);
    }

    startupModule(module);
}

/**
 * @brief Starts up Signup Module
 */
void MenuSystem::startupModuleSignup()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module(new ModSignup(m_session_data, m_config, m_ansi_process));
    if (!module)
    {
        std::cout << "ModSignup Allocation Error!" << std::endl;
        assert(false);
    }

    startupModule(module);
}

/**
 * @brief Handles Input for Login and PreLogin Sequences.
 * @param character_buffer
 * @param is_utf8
 */
void MenuSystem::handleLoginInputSystem(const std::string &character_buffer, const bool &is_utf8)
{
    // Make sure we have an allocated module before processing.
    if (m_module.size() == 0 || character_buffer.size() == 0)
    {
        return;
    }

    // Allocate and Create
    m_module[0]->update(character_buffer, is_utf8);

    // Finished modules processing.
    if (!m_module[0]->m_is_active)
    {
        shutdownModule();

        // Check if the current user has been logged in yet.
        if (!m_session_data->m_is_session_authorized)
        {            
            std::cout << "!m_is_session_authorized" << std::endl;
            m_current_menu = "matrix";
        }
        else
        {
            // If Authorized, then we want to move to main! Startup menu should be TOP or
            // Specified in Config file!  TODO
            std::cout << "m_is_session_authorized" << std::endl;
            m_current_menu = "main";
                     
        }
        
        loadAndStartupMenu();               
    }
}

/**
 * @brief Handles parsing input for preLogon module
 *
 */
void MenuSystem::modulePreLogonInput(const std::string &character_buffer, const bool &is_utf8)
{    
    std::cout << " *** modulePreLogonInput" << std::endl;    
    handleLoginInputSystem(character_buffer, is_utf8);
}

/**
 * @brief Handles parsing input for Logon module
 *
 */
void MenuSystem::moduleLogonInput(const std::string &character_buffer, const bool &is_utf8)
{   
    std::cout << " *** modulePreLogonInput" << std::endl;    
    handleLoginInputSystem(character_buffer, is_utf8);
}

/**
 * @brief Handles parsing input for modules
 *
 */
void MenuSystem::moduleInput(const std::string &character_buffer, const bool &is_utf8)
{
    std::cout << " *** moduleInput" << std::endl;
    
    // Make sure we have an allocated module before processing.
    if (m_module.size() == 0 || character_buffer.size() == 0)
    {
        return;
    }

    // Execute the modules update pass through input.
    m_module[0]->update(character_buffer, is_utf8);

    // Finished modules processing.
    if (!m_module[0]->m_is_active)
    {
        shutdownModule();

        // Reset the Input back to the Menu System
        resetMenuInputIndex(MENU_INPUT);

        // Redisplay,  may need to startup() again, but menu data should still be active and loaded!
        redisplayMenuScreen();
    }
}
