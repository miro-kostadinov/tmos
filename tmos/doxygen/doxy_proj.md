# Project managment {#doxy_proj}

**Topics:**

* @subpage doxy_proj_make
* @subpage doxy_proj_eclipse
* @subpage doxy_proj_naming

--------------------------------------------------------------------------------
# Naming Conventions {#doxy_proj_naming}

 For C functions:
 
  It is recommended to use names in the form **aaa_bbbb_cccc()** where **aaa** is
  optional function type or caller area.
  
  For example all **tsk_xxx()** functions can be called from tasks but cannot be
  called from service mode. While all **svc_xxx()** can be called from service
  mode only.
 
  When using multiple words for the function name they should be glued with "_",
  for example **string_cmpare()**. The word ordering sometimes can be wrong from
  the English grammar point of view. For example "green house" means that the
  object is a house which is supposed to be green. So if we have a function
  that returns such object one can expect that it should be named
  "green_house()".
  
  However, if we have a library for "house" related functions they all should
  be named as **house_xxx()**, so "house_green()" is preferred rather than
  "green_house()".
