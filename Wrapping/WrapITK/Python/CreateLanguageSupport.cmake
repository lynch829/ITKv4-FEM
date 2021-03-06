

macro(ADD_PYTHON_TYPEMAP simple_name cpp_name swig_name template_params_without_u)
  # TODO: remove this hack once bug #6177 is fixed
  string(REGEX REPLACE "(^| |<|,)([0-9]+)($| |>|,)" "\\1\\2u\\3" template_params "${template_params_without_u}")

  set(text "\n\n")
  if("${cpp_name}" STREQUAL "itk::SmartPointer")
    ADD_PYTHON_POINTER_TYPEMAP("${template_params}")
  endif("${cpp_name}" STREQUAL "itk::SmartPointer")

  if("${cpp_name}" STREQUAL "itk::Index")
    ADD_PYTHON_SEQ_TYPEMAP("${cpp_name}" "${template_params}")
  endif("${cpp_name}" STREQUAL "itk::Index")

  if("${cpp_name}" STREQUAL "itk::Size")
    ADD_PYTHON_SEQ_TYPEMAP("${cpp_name}" "${template_params}")
  endif("${cpp_name}" STREQUAL "itk::Size")

#   if("${cpp_name}" STREQUAL "itk::RGBPixel")
#     ADD_PYTHON_SEQ_TYPEMAP("${cpp_name}" "${template_params}")
#   endif("${cpp_name}" STREQUAL "itk::RGBPixel")

  if("${cpp_name}" STREQUAL "itk::Offset")
    ADD_PYTHON_SEQ_TYPEMAP("${cpp_name}" "${template_params}")
  endif("${cpp_name}" STREQUAL "itk::Offset")

  if("${cpp_name}" STREQUAL "itk::FixedArray")
    ADD_PYTHON_VEC_TYPEMAP("${cpp_name}" "${template_params}")
  endif("${cpp_name}" STREQUAL "itk::FixedArray")

  if("${cpp_name}" STREQUAL "itk::Vector")
    ADD_PYTHON_VEC_TYPEMAP("${cpp_name}" "${template_params}")
  endif("${cpp_name}" STREQUAL "itk::Vector")

  if("${cpp_name}" STREQUAL "itk::CovariantVector")
    ADD_PYTHON_VEC_TYPEMAP("${cpp_name}" "${template_params}")
  endif("${cpp_name}" STREQUAL "itk::CovariantVector")

  if("${cpp_name}" STREQUAL "itk::Point")
    ADD_PYTHON_VEC_TYPEMAP("${cpp_name}" "${template_params}")
  endif("${cpp_name}" STREQUAL "itk::Point")

  if("${cpp_name}" STREQUAL "itk::ContinuousIndex")
    ADD_PYTHON_VEC_TYPEMAP("${cpp_name}" "${template_params}")
  endif("${cpp_name}" STREQUAL "itk::ContinuousIndex")

  if("${cpp_name}" STREQUAL "itk::Array")
    ADD_PYTHON_VARIABLE_LENGHT_SEQ_TYPEMAP("${cpp_name}< ${template_params} >" "${template_params}")
  endif("${cpp_name}" STREQUAL "itk::Array")

endmacro(ADD_PYTHON_TYPEMAP)

macro(ADD_PYTHON_SEQ_TYPEMAP cpp_name dim)
  set(text "\n\n")
  set(text "${text}#ifdef SWIGPYTHON\n")
  set(text "${text}%typemap(in) ${cpp_name}<${dim}>& (${cpp_name}<${dim}> itks) {\n")
  set(text "${text}  if ((SWIG_ConvertPtr($input,(void **)(&$1),$1_descriptor, 0)) == -1) {\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}    if (PySequence_Check($input) && PyObject_Length($input) == ${dim}) {\n")
  set(text "${text}      for (int i =0; i < ${dim}; i++) {\n")
  set(text "${text}          PyObject *o = PySequence_GetItem($input,i);\n")
  set(text "${text}          if (!PyInt_Check(o)) {\n")
  set(text "${text}            PyErr_SetString(PyExc_ValueError,\"Expecting a sequence of int\");\n")
  set(text "${text}            return NULL;\n")
  set(text "${text}          }\n")
  set(text "${text}          itks[i] = PyInt_AsLong(o);\n")
  set(text "${text}      }\n")
  set(text "${text}      $1 = &itks;\n")
  set(text "${text}    }else if (PyInt_Check($input)) {\n")
  set(text "${text}      for (int i =0; i < ${dim}; i++) {\n")
  set(text "${text}          itks[i] = PyInt_AsLong($input);\n")
  set(text "${text}      }\n")
  set(text "${text}      $1 = &itks;\n")
  set(text "${text}    } else {\n")
  set(text "${text}      SWIG_fail;\n")
  set(text "${text}    }\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(typecheck) ${cpp_name}<${dim}>& {\n")
  set(text "${text}  void *ptr;\n")
  set(text "${text}  if (SWIG_ConvertPtr($input, &ptr, $1_descriptor, 0) == -1\n")
  set(text "${text}      && ( !PySequence_Check($input) || PyObject_Length($input) != ${dim} )\n")
  set(text "${text}      && !PyInt_Check($input) ) {\n")
  set(text "${text}    _v = 0;\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}  } else {\n")
  set(text "${text}    _v = 1;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(in) ${cpp_name}<${dim}> (${cpp_name}<${dim}> itks) {\n")
  set(text "${text}  ${cpp_name}<${dim}> * s;\n")
  set(text "${text}  if ((SWIG_ConvertPtr($input,(void **)(&s),$descriptor(${cpp_name}<${dim}>*), 0)) == -1) {\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}    if (PySequence_Check($input) && PyObject_Length($input) == ${dim}) {\n")
  set(text "${text}      for (int i =0; i < ${dim}; i++) {\n")
  set(text "${text}          PyObject *o = PySequence_GetItem($input,i);\n")
  set(text "${text}          if (!PyInt_Check(o)) {\n")
  set(text "${text}            PyErr_SetString(PyExc_ValueError,\"Expecting a sequence of int\");\n")
  set(text "${text}            return NULL;\n")
  set(text "${text}          }\n")
  set(text "${text}         itks[i] = PyInt_AsLong(o);\n")
  set(text "${text}      }\n")
  set(text "${text}      $1 = itks;\n")
  set(text "${text}    }else if (PyInt_Check($input)) {\n")
  set(text "${text}      for (int i =0; i < ${dim}; i++) {\n")
  set(text "${text}          itks[i] = PyInt_AsLong($input);\n")
  set(text "${text}      }\n")
  set(text "${text}      $1 = itks;\n")
  set(text "${text}    } else {\n")
  set(text "${text}      SWIG_fail;\n")
  set(text "${text}    }\n")
  set(text "${text}  } else if( s != NULL ) {\n")
  set(text "${text}    $1 = *s;\n")
  set(text "${text}  } else {\n")
  set(text "${text}    PyErr_SetString(PyExc_ValueError, \"Value can't be None\");\n")
  set(text "${text}    SWIG_fail;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(typecheck) ${cpp_name}<${dim}> {\n")
  set(text "${text}  void *ptr;\n")
  set(text "${text}  if (SWIG_ConvertPtr($input, &ptr, $descriptor(${cpp_name}<${dim}>*), 0) == -1\n")
  set(text "${text}       && ( !PySequence_Check($input) || PyObject_Length($input) != ${dim} )\n")
  set(text "${text}       && !PyInt_Check($input) ) {\n")
  set(text "${text}    _v = 0;\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}  } else {\n")
  set(text "${text}    _v = 1;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}#endif // SWIGPYTHON\n")
  set(text "${text}\n\n")

  set(WRAPPER_TYPEMAPS "${WRAPPER_TYPEMAPS}${text}")
endmacro(ADD_PYTHON_SEQ_TYPEMAP)

macro(ADD_PYTHON_VEC_TYPEMAP cpp_name template_params)
  string(REGEX REPLACE "(.*),(.*)" "\\1" type "${template_params}")
  string(REGEX REPLACE "(.*),(.*)" "\\2" dim "${template_params}")

  set(text "\n\n")
  set(text "${text}#ifdef SWIGPYTHON\n")
  set(text "${text}%typemap(in) ${cpp_name}<${template_params}>& (${cpp_name}<${template_params}> itks) {\n")
  set(text "${text}  if ((SWIG_ConvertPtr($input,(void **)(&$1),$1_descriptor, 0)) == -1) {\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}    if (PySequence_Check($input) && PyObject_Length($input) == ${dim}) {\n")
  set(text "${text}      for (int i =0; i < ${dim}; i++) {\n")
  set(text "${text}          PyObject *o = PySequence_GetItem($input,i);\n")
  set(text "${text}          if (PyInt_Check(o)) {\n")
  set(text "${text}            itks[i] = PyInt_AsLong(o);\n")
  set(text "${text}          } else if (PyFloat_Check(o)) {\n")
  set(text "${text}            itks[i] = (${type})PyFloat_AsDouble(o);\n")
  set(text "${text}          } else {\n")
  set(text "${text}            PyErr_SetString(PyExc_ValueError,\"Expecting a sequence of int or float\");\n")
  set(text "${text}            return NULL;\n")
  set(text "${text}          }\n")
  set(text "${text}      }\n")
  set(text "${text}      $1 = &itks;\n")
  set(text "${text}    }else if (PyInt_Check($input)) {\n")
  set(text "${text}      for (int i =0; i < ${dim}; i++) {\n")
  set(text "${text}          itks[i] = PyInt_AsLong($input);\n")
  set(text "${text}      }\n")
  set(text "${text}      $1 = &itks;\n")
  set(text "${text}    }else if (PyFloat_Check($input)) {\n")
  set(text "${text}      for (int i =0; i < ${dim}; i++) {\n")
  set(text "${text}          itks[i] = (${type})PyFloat_AsDouble($input);\n")
  set(text "${text}      }\n")
  set(text "${text}      $1 = &itks;\n")
  set(text "${text}    } else {\n")
  set(text "${text}      SWIG_fail;\n")
  set(text "${text}    }\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(typecheck) ${cpp_name}<${template_params}>& {\n")
  set(text "${text}  void *ptr;\n")
  set(text "${text}  if (SWIG_ConvertPtr($input, &ptr, $1_descriptor, 0) == -1\n")
  set(text "${text}      && ( !PySequence_Check($input) || PyObject_Length($input) != ${dim} )\n")
  set(text "${text}      && !PyInt_Check($input) && !PyFloat_Check($input) ) {\n")
  set(text "${text}    _v = 0;\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}  } else {\n")
  set(text "${text}    _v = 1;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(in) ${cpp_name}<${template_params}> (${cpp_name}<${template_params}> itks) {\n")
  set(text "${text}  ${cpp_name}<${template_params}> * s;\n")
  set(text "${text}  if ((SWIG_ConvertPtr($input,(void **)(&s),$descriptor(${cpp_name}<${template_params}>*), 0)) == -1) {\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}    if (PySequence_Check($input) && PyObject_Length($input) == ${dim}) {\n")
  set(text "${text}      for (int i =0; i < ${dim}; i++) {\n")
  set(text "${text}          PyObject *o = PySequence_GetItem($input,i);\n")
  set(text "${text}          if (PyInt_Check(o)) {\n")
  set(text "${text}            itks[i] = PyInt_AsLong(o);\n")
  set(text "${text}          } else if (PyFloat_Check(o)) {\n")
  set(text "${text}            itks[i] = (${type})PyFloat_AsDouble(o);\n")
  set(text "${text}          } else {\n")
  set(text "${text}            PyErr_SetString(PyExc_ValueError,\"Expecting a sequence of int or float\");\n")
  set(text "${text}            return NULL;\n")
  set(text "${text}          }\n")
  set(text "${text}      }\n")
  set(text "${text}      $1 = itks;\n")
  set(text "${text}    }else if (PyInt_Check($input)) {\n")
  set(text "${text}      for (int i =0; i < ${dim}; i++) {\n")
  set(text "${text}          itks[i] = PyInt_AsLong($input);\n")
  set(text "${text}      }\n")
  set(text "${text}      $1 = itks;\n")
  set(text "${text}    }else if (PyFloat_Check($input)) {\n")
  set(text "${text}      for (int i =0; i < ${dim}; i++) {\n")
  set(text "${text}          itks[i] = (${type})PyFloat_AsDouble($input);\n")
  set(text "${text}      }\n")
  set(text "${text}      $1 = itks;\n")
  set(text "${text}    } else {\n")
  set(text "${text}      SWIG_fail;\n")
  set(text "${text}    }\n")
  set(text "${text}  } else if( s != NULL ) {\n")
  set(text "${text}    $1 = *s;\n")
  set(text "${text} } else {\n")
  set(text "${text}   PyErr_SetString(PyExc_ValueError, \"Value can't be None\");\n")
  set(text "${text}   SWIG_fail;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(typecheck) ${cpp_name}<${template_params}> {\n")
  set(text "${text}  void *ptr;\n")
  set(text "${text}  if (SWIG_ConvertPtr($input, &ptr, $descriptor(${cpp_name}<${template_params}>*), 0) == -1\n")
  set(text "${text}       && ( !PySequence_Check($input) || PyObject_Length($input) != ${dim} )\n")
  set(text "${text}      && !PyInt_Check($input) && !PyFloat_Check($input) ) {\n")
  set(text "${text}    _v = 0;\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}  } else {\n")
  set(text "${text}    _v = 1;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}#endif // SWIGPYTHON\n")
  set(text "${text}\n\n")

  set(WRAPPER_TYPEMAPS "${WRAPPER_TYPEMAPS}${text}")
endmacro(ADD_PYTHON_VEC_TYPEMAP)

macro(ADD_PYTHON_VARIABLE_LENGHT_SEQ_TYPEMAP type value_type)
  set(text "\n\n")
  set(text "${text}#ifdef SWIGPYTHON\n")
  set(text "${text}%typemap(in) ${type}& (${type} itks) {\n")
  set(text "${text}  if ((SWIG_ConvertPtr($input,(void **)(&$1),$1_descriptor, 0)) == -1) {\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}    itks = ${type}( PyObject_Length($input) );\n")
  set(text "${text}    for (int i =0; i < itks.GetSize(); i++) {\n")
  set(text "${text}      PyObject *o = PySequence_GetItem($input,i);\n")
  set(text "${text}      if (PyInt_Check(o)) {\n")
  set(text "${text}        itks[i] = (${value_type})PyInt_AsLong(o);\n")
  set(text "${text}      } else if (PyFloat_Check(o)) {\n")
  set(text "${text}        itks[i] = (${value_type})PyFloat_AsDouble(o);\n")
  set(text "${text}      } else {\n")
  set(text "${text}        PyErr_SetString(PyExc_ValueError,\"Expecting a sequence of int or float\");\n")
  set(text "${text}        return NULL;\n")
  set(text "${text}      }\n")
  set(text "${text}    }\n")
  set(text "${text}    $1 = &itks;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(typecheck) ${type}& {\n")
  set(text "${text}  void *ptr;\n")
  set(text "${text}  if (SWIG_ConvertPtr($input, &ptr, $1_descriptor, 0) == -1\n")
  set(text "${text}      && !PySequence_Check($input) ) {\n")
  set(text "${text}    _v = 0;\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}  } else {\n")
  set(text "${text}    _v = 1;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(in) ${type} (${type} itks) {\n")
  set(text "${text}  ${type} * s;\n")
  set(text "${text}  if ((SWIG_ConvertPtr($input,(void **)(&s),$descriptor(${type}*), 0)) == -1) {\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}    itks = ${type}( PyObject_Length($input) );\n")
  set(text "${text}    for (int i =0; i < itks.GetSize(); i++) {\n")
  set(text "${text}      PyObject *o = PySequence_GetItem($input,i);\n")
  set(text "${text}      if (PyInt_Check(o)) {\n")
  set(text "${text}        itks[i] = (${value_type})PyInt_AsLong(o);\n")
  set(text "${text}      } else if (PyFloat_Check(o)) {\n")
  set(text "${text}        itks[i] = (${value_type})PyFloat_AsDouble(o);\n")
  set(text "${text}      } else {\n")
  set(text "${text}        PyErr_SetString(PyExc_ValueError,\"Expecting a sequence of int or float\");\n")
  set(text "${text}        return NULL;\n")
  set(text "${text}      }\n")
  set(text "${text}    }\n")
  set(text "${text}    $1 = itks;\n")
  set(text "${text}  } else {\n")
  set(text "${text}  $1 = *s;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(typecheck) ${type} {\n")
  set(text "${text}  void *ptr;\n")
  set(text "${text}  if (SWIG_ConvertPtr($input, &ptr, $descriptor(${type}*), 0) == -1\n")
  set(text "${text}      && !PySequence_Check($input) ) {\n")
  set(text "${text}    _v = 0;\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}  } else {\n")
  set(text "${text}    _v = 1;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}#endif // SWIGPYTHON\n")
  set(text "${text}\n\n")

  set(WRAPPER_TYPEMAPS "${WRAPPER_TYPEMAPS}${text}")
endmacro(ADD_PYTHON_VARIABLE_LENGHT_SEQ_TYPEMAP)


macro(ADD_PYTHON_POINTER_TYPEMAP template_params)
  set(text "\n\n")
  set(text "${text}#ifdef SWIGPYTHON\n")
  set(text "${text} // Python typemaps for Smart Pointers to ${template_params} class. \n\n")
  set(text "${text}%typemap(out) ${template_params} * {\n")
  set(text "${text}  std::string methodName = \"\$symname\";\n")
  set(text "${text}  if(methodName.find(\"GetPointer\") != std::string::npos) {\n")
  set(text "${text}    // really return a pointer in that case\n")
  set(text "${text}    \$result = SWIG_NewPointerObj((void *)(\$1), \$1_descriptor, 1);\n")
  set(text "${text}  } else {\n")
  set(text "${text}    itk::SmartPointer< ${template_params} > * ptr;\n")
  set(text "${text}    ptr = new itk::SmartPointer< ${template_params} >(\$1);\n")
  set(text "${text}    \$result = SWIG_NewPointerObj((void *)(ptr), \$descriptor(itk::SmartPointer< ${template_params} > *), 1);\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(in) ${template_params} * {\n")
  set(text "${text}  itk::SmartPointer< ${template_params} > * sptr;\n")
  set(text "${text}  ${template_params} * ptr;\n")
  set(text "${text}  // we must test this case first to avoid a segfault if arg is None\n")
  set(text "${text}  if ((SWIG_ConvertPtr($input,(void **) &ptr, $1_descriptor, SWIG_POINTER_EXCEPTION)) != -1) {\n")
  set(text "${text}    // we have a simple pointer. Just return it\n")
  set(text "${text}    $1 = ptr;\n")
  set(text "${text}  } else if ((SWIG_ConvertPtr($input,(void **) &sptr, $descriptor(itk::SmartPointer< ${template_params} > *), SWIG_POINTER_EXCEPTION)) != -1) {\n")
  set(text "${text}    // it's a SmartPointer. Get the pointer and return it\n")
  set(text "${text}    $1 = sptr->GetPointer();\n")
  set(text "${text}  } else {\n")
  set(text "${text}    // not a pointer nor a SmartPointer... typemap fail !\n")
  set(text "${text}    SWIG_fail;\n")
  set(text "${text}  }\n")
  set(text "${text}  // clean the error before exit\n")
  set(text "${text}  PyErr_Clear();\n")
  set(text "${text}}\n")
  set(text "${text}%typemap(typecheck) ${template_params} * {\n")
  set(text "${text}  void *ptr;\n")
  set(text "${text}  if (SWIG_ConvertPtr(\$input, &ptr, \$1_descriptor, 0) == -1\n")
  set(text "${text}      && SWIG_ConvertPtr(\$input, &ptr, \$descriptor(itk::SmartPointer< ${template_params} > *), 0) == -1) {\n")
  set(text "${text}    _v = 0;\n")
  set(text "${text}    PyErr_Clear();\n")
  set(text "${text}  } else {\n")
  set(text "${text}    _v = 1;\n")
  set(text "${text}  }\n")
  set(text "${text}}\n")
  set(text "${text}#endif // SWIGPYTHON\n")
  set(text "${text}\n\n")

  set(WRAPPER_TYPEMAPS "${WRAPPER_TYPEMAPS}${text}")
endmacro(ADD_PYTHON_POINTER_TYPEMAP)


macro(CONFIGURE_PYTHON_CONFIG_FILES outdir)
  # Pull the WRAPPED_CLASSES list apart and use it to create Python-specific
  # support files. This also uses the global WRAPPER_LIBRARY_DEPENDS,
  # and WRAPPER_LIBRARY_NAME variables.

    set(CONFIG_DEPENDS "")
  foreach(dep ${WRAPPER_LIBRARY_DEPENDS})
    set(CONFIG_DEPENDS "${CONFIG_DEPENDS} '${dep}',")
  endforeach(dep)

  # Deal with the WRAPPED_CLASSES strings, which are in the format
  # "simple name # c++ name # swig name # c++ template parameters"
  # or "simple name # c++ name # swig name # NO_TEMPLATE"
  # We want to change them to the format:
  # "  ('simple name', 'c++ name', 'swig name', 'template params'),\n"
  # or "  ('simple name', 'c++ name', 'swig name'),\n", respectively.

  set(CONFIG_TEMPLATES "")
  foreach(wrapped_class ${WRAPPED_CLASSES})
    # first put the internal quotes in place
    string(REGEX REPLACE
      " # "
      "', '"
      py_template_def
      "${wrapped_class}")
    # now put the outside parens and quotes, etc. in place
    set(py_template_def "  ('${py_template_def}'),\n")
    # now strip out the NO_TEMPLATE if there is none
    string(REGEX REPLACE
      ", 'NO_TEMPLATE'"
      ""
      py_template_def
      "${py_template_def}")
    set(CONFIG_TEMPLATES "${CONFIG_TEMPLATES}${py_template_def}")
  endforeach(wrapped_class)

  configure_file("${WRAP_ITK_CONFIG_DIR}/LanguageSupport/ModuleConfig.py.in"
    "${outdir}/${WRAPPER_LIBRARY_NAME}Config.py"
    @ONLY IMMEDIATE)
  WRAP_ITK_INSTALL("/Python/Configuration" "${outdir}/${WRAPPER_LIBRARY_NAME}Config.py")
endmacro(CONFIGURE_PYTHON_CONFIG_FILES)

macro(CONFIGURE_PYTHON_LOADER_FILE outdir)
  # Create the loader file for importing just the current wrapper library. Uses
  # the global WRAPPER_LIBRARY_NAME variable.

  set(CONFIG_LIBRARY_NAME "${WRAPPER_LIBRARY_NAME}")
  configure_file("${WRAP_ITK_CONFIG_DIR}/LanguageSupport/ModuleLoader.py.in"
    "${outdir}/${WRAPPER_LIBRARY_NAME}.py"
    @ONLY IMMEDIATE)
endmacro(CONFIGURE_PYTHON_LOADER_FILE)

macro(CONFIGURE_PYTHON_EXTERNAL_PROJECT_CONFIG outdir)
  # Create a helper file to set some sys.path entries so that external projects
  # can be easily loaded even when not installed. To use, call 'import ProjectConfig'
  # or 'import ProjectConfig-[Debug|Release|...]' if multiple build styles were
  # selected from an IDE. After this module is imported, the external project
  # can be easily imported by 'import ProjectName'.
  # If ProjectConfig was not used, the user would have to manually set sys.path
  # to point to *both* the Python directory in the WrapITK build tree and the
  # directory where the current project's SWIG libraries have been placed.

  if(CMAKE_CONFIGURATION_TYPES)
    foreach(config ${CMAKE_CONFIGURATION_TYPES})
      # SWIG-generated libs and *.py files are sent to ${config} subdir
      set(CONFIG_WRAP_ITK_PYTHON_DIR "${WrapITK_DIR}/Python/${config}")
      set(CONFIG_PROJECT_PYTHON_DIR "${CMAKE_BINARY_DIR}/Python/${config}")
      set(CONFIG_PROJECT_OUTPUT_DIR "${LIBRARY_OUTPUT_PATH}/${config}")
      configure_file("${WRAP_ITK_CONFIG_DIR}/LanguageSupport/ExternalProjectConfig.py.in"
        "${outdir}/ProjectConfig_${config}.py"
        @ONLY IMMEDIATE)
      configure_file("${WRAP_ITK_CONFIG_DIR}/LanguageSupport/ExternalProjectConfig.pth.in"
        "${outdir}/${config}/${PROJECT_NAME}.pth"
        @ONLY IMMEDIATE)
  endforeach(config)
  else(CMAKE_CONFIGURATION_TYPES)
    set(CONFIG_WRAP_ITK_PYTHON_DIR "${WrapITK_DIR}/Python")
    set(CONFIG_PROJECT_PYTHON_DIR "${CMAKE_BINARY_DIR}/Python")
    set(CONFIG_PROJECT_OUTPUT_DIR "${LIBRARY_OUTPUT_PATH}")
    configure_file("${WRAP_ITK_CONFIG_DIR}/LanguageSupport/ExternalProjectConfig.py.in"
      "${outdir}/ProjectConfig.py"
      @ONLY IMMEDIATE)
    configure_file("${WRAP_ITK_CONFIG_DIR}/LanguageSupport/ExternalProjectConfig.pth.in"
      "${outdir}/${PROJECT_NAME}.pth"
      @ONLY IMMEDIATE)
  endif(CMAKE_CONFIGURATION_TYPES)
endmacro(CONFIGURE_PYTHON_EXTERNAL_PROJECT_CONFIG)


macro(PYTHON_SUPPORT_CONFIGURE_FILES)
  # Create the various files to make it easier to use the ITK wrappers, especially
  # with reference to the multitude of templates.
  # Currently, only Python is supported.

    CONFIGURE_PYTHON_CONFIG_FILES("${PROJECT_BINARY_DIR}/Python/Configuration")
    if(CMAKE_CONFIGURATION_TYPES)
      foreach(config ${CMAKE_CONFIGURATION_TYPES})
        CONFIGURE_PYTHON_LOADER_FILE("${PROJECT_BINARY_DIR}/Python/${config}")
      endforeach(config)
    else(CMAKE_CONFIGURATION_TYPES)
      CONFIGURE_PYTHON_LOADER_FILE("${PROJECT_BINARY_DIR}/Python/")
    endif(CMAKE_CONFIGURATION_TYPES)
    # Just install the files once, regardless of how many different places
    # they were configured into. If there are no configuration types, the
    # INTDIR variable expands to '', so no harm done.
    WRAP_ITK_INSTALL("/Python" "${PROJECT_BINARY_DIR}/Python/${WRAP_ITK_INSTALL_INTDIR}${WRAPPER_LIBRARY_NAME}.py")
    if(EXTERNAL_WRAP_ITK_PROJECT)
      # Configure a python file to make it easier to use this external project
      # without first installing it into WrapITK.
      CONFIGURE_PYTHON_EXTERNAL_PROJECT_CONFIG("${PROJECT_BINARY_DIR}/Python/")
    endif(EXTERNAL_WRAP_ITK_PROJECT)
endmacro(PYTHON_SUPPORT_CONFIGURE_FILES)


