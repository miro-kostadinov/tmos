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
 *
 *
 */

