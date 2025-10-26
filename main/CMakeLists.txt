idf_component_register(SRCS "main.c" "app_disp_fs.c" "speech_recognition.cpp" "model.cc" "micro_features_generator.cc"
                    INCLUDE_DIRS ".")

# Reduce the level of paranoia to be able to compile sources
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wno-maybe-uninitialized
    -Wno-missing-field-initializers
    -Wno-error=sign-compare
    -Wno-error=double-promotion
    -Wno-type-limits)
