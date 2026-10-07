#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	switch (p_level) {
	case MODULE_INITIALIZATION_LEVEL_SERVERS: {
		// TODO: GDREGISTER_CLASS for server types here
		break;
	}
	case MODULE_INITIALIZATION_LEVEL_SCENE: {
		// TODO: GDREGISTER_CLASS for node types here
		print_line("Hello from GDExtension!");
		break;
	}
	default: break;
	}
}

void uninitialize_gdextension_types([[maybe_unused]] ModuleInitializationLevel p_level) {
}

extern "C" {
// GDExtension entry symbol
// TODO: Replace "gdextension_cmake_template" with your addon or module name; modify the manifest accordingly
GDExtensionBool GDE_EXPORT gdextension_cmake_template_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
