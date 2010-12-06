/**
 * @file mainpage.h
 * @brief file used for doxygen
 *
 *  Created on: 16.11.2010
 *      Author: Miroslav Kostadinov
 *
 *
 *
 *
 * @mainpage TMOS operating system
 *
 * @section project_sec Introduction
 *
 *   TMOS introduction here...
 *
 *   The project structure
 *
 *   @subsection make_ssec make system
 *   @subsection cores_ssec cores
 *   @subsection drivers_ssec drivers
 *   @subsection services_ssec services
 *
 *
 * @page eclipse Eclipse
 *
 * @section eclipse_settings Recommended settings
 *
 * - Enable Doxygen Documentation tool comments
 * Goto Window->Preferences [C/C++ -> Editor] and from the dropdown list
 * "Windows default" change from none to Doxygen. Press apply. With this option
 * selected the doxygen comments are colored in blue (not green). Also when you
 * press /** and then enter before a function you will get default params ets.
 *
 * - Enable Show print margin
 * Goto Window->Preferences [General->Editors->Text Editors] and select show
 * print margin.
 * A ruler line will be shown indicating the print margin after 80 columns. Try
 * to keep the sources and comments within this margin.
 *
 * @section eclipse_warnings Warnings
 * Invalid project path: Duplicate path entries found (/tmos [Output path])
 *
 * The reason for this warning is that multiple compilation use the same output
 * path (out/xxx).
 *
 * Workaround:
 * Go to project settings->C/C++ General->Paths and Symbols And for every
 * configuration (board) click on "Output Location" tab.
 * First press Add Folder and select the correct output path. For example if the
 * board is my_board, the folder should be "tmos/out/my_board". Then delete the
 * default folder (tmos/).
 *
 *
 *@page nam_conv Naming conventions
 *
 * @par For C functions
 *
 * It is recommended to use names in the form "aaa_bbbb_cccc()" where "aaa" is
 * optional function type or caller area.
 * For example all "tsk_xxx()" functions can be called from tasks but cannot be
 * called from service mode. While all "svc_xxx()" can be called from service
 * mode only.
 *
 * When using multiple words for the function name they should be glued with "_",
 * for example "string_cmpare()". The word ordering sometimes can be wrong from
 * the English grammar point of view. For example "green house" means that the
 * object is a house which is supposed to be green. So if we have a function
 * that returns such object one can expect that it should be called
 * "green_house()".
 * However, if we have a library for "house" related functions they all should
 * be named as "house_xxx()", so "house_green()" is preferred rather than
 * "green_house()".
 *
 */

