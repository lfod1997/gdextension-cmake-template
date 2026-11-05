# GDExtension CMake Template

Template project for GDExtension development with CMake.

An alternative to the official [godot-cpp-template](https://github.com/godotengine/godot-cpp-template), **targeting Godot 4.3+ and godot-cpp 10**.

## ~~Quick~~ Slow Start

1. Do either of:
    1. Log in to GitHub and select "Use this template" at the top of the homepage of this repository
    2. Clone this repository and run `git remote remove origin`
2. Modify the CMakeLists.txt file, do either of:
    1. If you're making an addon (i.e. your work is reusable, and possibly facing the public), change the values assigned to `LIBNAME` and `ADDON`:
        ```cmake
        set(LIBNAME "YOUR_LIBRARY_NAME") # A convention is to start with "gd" and use lower case, eg. "gdcoolthing"
        set(ADDON "YOUR_ADDON_NAME") # Use the name of your Git repo, etc.
        ```
    2. If your GDExtension is "the native part" of your game (i.e. its usage is limited to one specific project), *remove* the value of `ADDON`, and optionally modify the value of `LIBNAME` (do, if you plan to create multiple GDExtension modules for your game):
        ```cmake
        set(LIBNAME "gdext") # The name "gdext" is okay; feel free to change
        set(ADDON "") # Or just delete this line
        ```
3. Open the file src/register_types.cpp, rename the function `gdextension_cmake_template_init` to something else (the convention is to end with `_init`); we call it "the entry symbol"
4. Modify gdext.gdextension ("the manifest"). Check BOTH of:
    1. If you changed `LIBNAME`, rename gdext.gdextension to "VALUE_OF_LIBNAME.gdextension" AND, in the file, search for "gdext" and replace with the value of `LIBNAME`
    2. Change the value of `entry_symbol` to "the entry symbol" invented in step 3
5. The CMake project is ready to be configured

The default place to put the artifacts (the manifest and all binaries) is under **project/**: an empty folder (apart from Git config files) under which you may create a new Godot project:

1. Open the Godot editor
2. At the Project Manager, select "Create"
3. *First*, turn off the "Create Folder" switch
4. Browse or enter the absolute path to the **project/** folder
5. Select: "Version Control Metadata" \> "**None**"
6. You can now build the CMake target "gdext", and see it loaded into the Godot project

Alternatively, you can instruct CMake to put the artifacts elsewhere, by changing the CMake config argument `GODOT_PROJECT_DIR`:

```
-DGODOT_PROJECT_DIR='path/to/existing/godot/project'
```

Reconfigure CMake project to make effect.

> If you're using CLion, CMake config arguments can be modified in "Settings" \> "Build, Execution, Deployment" \> "CMake" \> (active profile) \> "CMake options:".

## Targeting a Godot Version

The upstream has stated [in their README.md](https://github.com/godotengine/godot-cpp#versioning) that:

> Starting with version 10.x, godot-cpp is versioned independently of Godot. Using the `api_version` parameter (see below), godot-cpp v10 can target Godot 4.3 or later.

"api_version" is a SCons setting; we'll need its CMake counterpart `GODOTCPP_API_VERSION`. This template targets 4.5, which you can change in the CMakeLists.txt:

```cmake
set(GODOTCPP_API_VERSION 4.4) # Target Godot 4.4 and later
```

If changed, the value assigned to `compatibility_minimum` in the manifest file also needs to change.

## Adding Dependencies in CMake

Other CMake modules may be added like you would in a normal CMake project.

### Using FetchContent

If the dependency supports `FetchContent`, declared like:

```cmake
FetchContent_Declare(
    CoolPackage
    GIT_REPOSITORY https://example.com/cool.git
    GIT_TAG "1.2.0"
    GIT_SHALLOW TRUE
)
```

It's important that you put `FetchContent_MakeAvailable` of the dependency *after* `FetchContent_MakeAvailable(godot-cpp)`.

### Adding a Subdirectory

If there's a CMake module located in your project, you may want to `add_subdirectory`. Make sure to put it *after* `FetchContent_MakeAvailable(godot-cpp)`.

### Through Find-Package API

If you need to `find_package`, make sure to put it *after* project declaration (the `project(...)` call).
