// https://github.com/oov/aviutl2_plugin_sdk_for_c
// The MIT License / Copyright (c) 2025 oov
#pragma once

// This file based on plugin2.h from AviUtl ExEdit2 Plugin SDK
// LICENSE: The MIT License / Copyright (c) 2025 Kenkun

// Plugins must export the following functions for the host to call:
//
// Required plugin registration function
//   void RegisterPlugin(struct aviutl2_host_app_table *host)
//
// Optional plugin DLL initialization function (version is the host application version)
//   bool InitializePlugin(uint32_t version)
//
// Optional plugin DLL uninitialization function
//   void UninitializePlugin(void)
//
// Optional logger initialization function (see aviutl2_logger2.h). Called before InitializePlugin()
//   void InitializeLogger(struct aviutl2_log_handle *logger)
//
// Optional config initialization function (see aviutl2_config2.h). Called before InitializePlugin()
//   void InitializeConfig(struct aviutl2_config_handle *config)

#include <stdbool.h>
#include <stdint.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

struct aviutl2_input_plugin_table;
struct aviutl2_output_plugin_table;
struct aviutl2_filter_plugin_table;
struct aviutl2_script_module_table;
struct aviutl2_edit_handle;
struct aviutl2_project_file;

/**
 * Object handle
 */
typedef void *aviutl2_object_handle;

/**
 * Object layer and frame information
 * In object frames, frame numbers and layer numbers start from 0 (different from UI display)
 */
struct aviutl2_object_layer_frame {
  int layer; /**< Layer number */
  int start; /**< Start frame number */
  int end;   /**< End frame number */
};

/**
 * Media information
 */
struct aviutl2_media_info {
  int video_track_num; /**< Video track count (0 if no video) */
  int audio_track_num; /**< Audio track count (0 if no audio) */
  double total_time;   /**< Total time (0 for still images) */
  int width, height;   /**< Resolution */
};

/**
 * Module type constants
 */
enum {
  aviutl2_module_type_script_filter = 1, /**< Filter script */
  aviutl2_module_type_script_object = 2, /**< Object script */
  aviutl2_module_type_script_camera = 3, /**< Camera script */
  aviutl2_module_type_script_track = 4,  /**< Track bar script */
  aviutl2_module_type_script_module = 5, /**< Script module */
  aviutl2_module_type_plugin_input = 6,  /**< Input plugin */
  aviutl2_module_type_plugin_output = 7, /**< Output plugin */
  aviutl2_module_type_plugin_filter = 8, /**< Filter plugin */
  aviutl2_module_type_plugin_common = 9, /**< Common plugin */
};

/**
 * Module information
 */
struct aviutl2_module_info {
  int type;                    /**< Module type (aviutl2_module_type_*) */
  wchar_t const *name;         /**< Module name */
  wchar_t const *information;  /**< Module information */
};

//--------------------------------

/**
 * Edit information
 * In edit info, frame numbers and layer numbers start from 0 (different from UI display)
 */
struct aviutl2_edit_info {
  int width, height;        /**< Scene resolution */
  int rate, scale;          /**< Scene frame rate */
  int sample_rate;          /**< Scene sampling rate */
  int frame;                /**< Current cursor frame number */
  int layer;                /**< Current selected layer number */
  int frame_max;            /**< Maximum frame number where objects exist */
  int layer_max;            /**< Maximum layer number where objects exist */
  int display_frame_start;  /**< Start frame number displayed in layer editor */
  int display_layer_start;  /**< Start layer number displayed in layer editor */
  int display_frame_num;    /**< Number of frames displayed in layer editor (not exact) */
  int display_layer_num;    /**< Number of layers displayed in layer editor (not exact) */
  int select_range_start;   /**< Start frame number of frame range selection (-1 if not selected) */
  int select_range_end;     /**< End frame number of frame range selection (-1 if not selected) */
  float grid_bpm_tempo;     /**< Grid(BPM) tempo */
  int grid_bpm_beat;        /**< Grid(BPM) beat */
  float grid_bpm_offset;    /**< Grid(BPM) base time */
  int scene_id;             /**< Scene ID */
};

/**
 * Edit section
 * In edit section, frame numbers and layer numbers start from 0 (different from UI display)
 */
struct aviutl2_edit_section {
  /**
   * Edit information
   */
  struct aviutl2_edit_info *info;

  /**
   * Create object alias at specified position
   * @param alias Pointer to object alias data (UTF-8). Same format as object alias file (.object)
   * @param layer Layer number to create
   * @param frame Frame number to create
   * @param length Frame count of object. If frame information exists in alias data, length is set from that frame information.
   *               If 0 is specified for frame count, length or add position are auto-adjusted
   * @return Handle of created object (returns NULL on failure). Fails if overlapping with existing object or if alias
   * data is invalid. For multi-object alias data, returns handle of the first object. All objects are created.
   */
  aviutl2_object_handle (*create_object_from_alias)(char const *alias, int layer, int frame, int length);

  /**
   * Find object at specified frame number or later
   * @param layer Target layer number
   * @param frame Frame number to start search from
   * @return Handle of found object (returns NULL if not found)
   */
  aviutl2_object_handle (*find_object)(int layer, int frame);

  /**
   * Get number of target effects in object
   * @param object Object handle
   * @param effect Target effect name (effect.name value in alias file)
   * @return Number of target effects (0 if not found)
   */
  int (*count_object_effect)(aviutl2_object_handle object, wchar_t const *effect);

  /**
   * Get object layer and frame information
   * @param object Object handle
   * @return Object layer and frame information
   */
  struct aviutl2_object_layer_frame (*get_object_layer_frame)(aviutl2_object_handle object);

  /**
   * Get object alias data
   * @param object Object handle
   * @return Pointer to object alias data (UTF-8) (returns NULL if cannot be obtained).
   *         Same format as object alias file.
   *         Returned string is valid until the end of callback function that uses this function
   */
  char const *(*get_object_alias)(aviutl2_object_handle object);

  /**
   * Get object configuration item value as string
   * @param object Object handle
   * @param effect Target effect name (effect.name value in alias file)
   * @param item Target configuration item name (key name in alias file)
   * @return Pointer to obtained configuration value (UTF-8) (returns NULL if cannot be obtained).
   *         Same format as configuration value in alias file.
   *         Returned string is valid until the end of callback function that uses this function
   */
  char const *(*get_object_item_value)(aviutl2_object_handle object, wchar_t const *effect, wchar_t const *item);

  /**
   * Set object configuration item value as string
   * @param object Object handle
   * @param effect Target effect name (effect.name value in alias file)
   * @param item Target configuration item name (key name in alias file)
   * @param value Configuration value (UTF-8). Same format as configuration value in alias file
   * @return true if configuration succeeded (fails if target does not exist)
   */
  bool (*set_object_item_value)(aviutl2_object_handle object,
                                wchar_t const *effect,
                                wchar_t const *item,
                                char const *value);

  /**
   * Move object
   * @param object Object handle
   * @param layer Destination layer number
   * @param frame Destination frame number
   * @return true if move succeeded (fails if object exists at destination)
   */
  bool (*move_object)(aviutl2_object_handle object, int layer, int frame);

  /**
   * Delete object
   * @param object Object handle
   */
  void (*delete_object)(aviutl2_object_handle object);

  /**
   * Get handle of currently selected object in object configuration window
   * @return Object handle (returns NULL if no selection)
   */
  aviutl2_object_handle (*get_focus_object)(void);

  /**
   * Set object to be selected in object configuration window (set at end of callback)
   * @param object Object handle
   */
  void (*set_focus_object)(aviutl2_object_handle object);

  /**
   * Get pointer to project file
   * @param edit Edit handle
   * @return Pointer to project file structure. Valid until callback processing ends
   */
  struct aviutl2_project_file *(*get_project_file)(struct aviutl2_edit_handle *edit);

  /**
   * Get handle of selected object in layer editor by index
   * @param index Selection index (0 based)
   * @return Object handle (NULL if index is out of range)
   */
  aviutl2_object_handle (*get_selected_object)(int index);

  /**
   * Get number of selected objects in layer editor
   * @return Number of selected objects
   */
  int (*get_selected_object_num)(void);

  /**
   * Get layer and frame position from mouse coordinates
   * Calculates from coordinates of the last mouse move window message
   * @param layer Pointer to store layer number
   * @param frame Pointer to store frame number
   * @return true if mouse coordinates are on layer editor
   */
  bool (*get_mouse_layer_frame)(int *layer, int *frame);

  /**
   * Get layer and frame position from specified screen coordinates
   * @param x Screen X coordinate
   * @param y Screen Y coordinate
   * @param layer Pointer to store layer number
   * @param frame Pointer to store frame number
   * @return true if screen coordinates are on layer editor
   */
  bool (*pos_to_layer_frame)(int x, int y, int *layer, int *frame);

  /**
   * Check if specified media file is supported
   * @param file Media file path
   * @param strict If true, check if file can actually be loaded. If false, check if extension is supported
   * @return true if supported
   */
  bool (*is_support_media_file)(wchar_t const *file, bool strict);

  /**
   * Get information of specified media file
   * Cannot get info for files other than video, audio, and image files
   * @param file Media file path
   * @param info Pointer to media info storage
   * @param info_size Size of media info storage (only size bytes are retrieved if different from MEDIA_INFO)
   * @return true if info was obtained
   */
  bool (*get_media_info)(wchar_t const *file, struct aviutl2_media_info *info, int info_size);

  /**
   * Create object from media file at specified position
   * @param file Media file path
   * @param layer Layer number to create
   * @param frame Frame number to create
   * @param length Frame count of object. If 0 is specified, length or position are auto-adjusted
   * @return Handle of created object (returns NULL on failure)
   *         Fails if overlapping with existing object or if media file is not supported
   */
  aviutl2_object_handle (*create_object_from_media_file)(wchar_t const *file, int layer, int frame, int length);

  /**
   * Create object at specified position
   * @param effect Effect name (effect.name value in alias file)
   * @param layer Layer number to create
   * @param frame Frame number to create
   * @param length Frame count of object. If 0 is specified, length or position are auto-adjusted
   * @return Handle of created object (returns NULL on failure)
   *         Fails if overlapping with existing object or if specified effect is not supported
   */
  aviutl2_object_handle (*create_object)(wchar_t const *effect, int layer, int frame, int length);

  /**
   * Set current layer/frame position. Adjusted to available range
   * @param layer Layer number
   * @param frame Frame number
   */
  void (*set_cursor_layer_frame)(int layer, int frame);

  /**
   * Set display start position of layer/frame in layer editor. Adjusted to available range
   * @param layer Display start layer number
   * @param frame Display start frame number
   */
  void (*set_display_layer_frame)(int layer, int frame);

  /**
   * Set frame range selection. Adjusted to available range
   * @param start Start frame number
   * @param end End frame number. Specifying -1 for both start and end clears the selection
   */
  void (*set_select_range)(int start, int end);

  /**
   * Set grid(BPM)
   * @param tempo Tempo
   * @param beat Beat
   * @param offset Base time
   */
  void (*set_grid_bpm)(float tempo, int beat, float offset);

  /**
   * Get object name
   * @param object Object handle
   * @return Pointer to object name (returns NULL if using standard name).
   *         Valid until object is edited or callback processing ends
   */
  wchar_t const *(*get_object_name)(aviutl2_object_handle object);

  /**
   * Set object name
   * @param object Object handle
   * @param name Object name (specifying NULL or empty string sets to standard name)
   */
  void (*set_object_name)(aviutl2_object_handle object, wchar_t const *name);
};

/**
 * Edit handle
 */
struct aviutl2_edit_handle {
  /**
   * Call callback function (func_proc_edit) to edit project data
   * Callback function is called every time edit is repeated
   * Objects edited in callback function are automatically registered to Undo
   * Callback function is called from main thread
   * @param func_proc_edit Callback function for editing
   * @return true on success. Fails if edit is not available (during output)
   */
  bool (*call_edit_section)(void (*func_proc_edit)(struct aviutl2_edit_section *edit));

  /**
   * Call callback function (func_proc_edit) to edit project data with parameter
   * @param param Pointer to arbitrary user data
   * @param func_proc_edit Callback function for editing
   * @return true on success. Fails if edit is not available (during output)
   */
  bool (*call_edit_section_param)(void *param, void (*func_proc_edit)(void *param, struct aviutl2_edit_section *edit));

  /**
   * Get edit information
   * Cannot be used if edit processing is already in progress (inside callback function with EDIT_SECTION argument etc.)
   * Deadlocks if called in such situation
   * @param info Pointer to edit info storage
   * @param info_size Size of edit info storage (only size bytes are retrieved if different from EDIT_INFO)
   */
  void (*get_edit_info)(struct aviutl2_edit_info *info, int info_size);

  /**
   * Restart the host application
   */
  void (*restart_host_app)(void);

  /**
   * Enumerate effect names via callback function (func_proc_enum_effect)
   * @param param Pointer to arbitrary user data
   * @param func_proc_enum_effect Callback function for effect name enumeration
   */
  void (*enum_effect_name)(void *param,
                           void (*func_proc_enum_effect)(void *param, wchar_t const *name, int type, int flag));

  /**
   * Enumerate module information via callback function (func_proc_enum_module)
   * @param param Pointer to arbitrary user data
   * @param func_proc_enum_module Callback function for module information enumeration
   */
  void (*enum_module_info)(void *param,
                           void (*func_proc_enum_module)(void *param, struct aviutl2_module_info *info));
};

/**
 * Effect type constants (may be extended in the future)
 */
enum {
  aviutl2_effect_type_filter = 1,     /**< Filter effect */
  aviutl2_effect_type_input = 2,      /**< Media input */
  aviutl2_effect_type_transition = 3, /**< Scene change */
};

/**
 * Effect flag constants (may be extended in the future)
 */
enum {
  aviutl2_effect_flag_video = 1,  /**< Supports video */
  aviutl2_effect_flag_audio = 2,  /**< Supports audio */
  aviutl2_effect_flag_filter = 4, /**< Supports filter object */
};

//--------------------------------

/**
 * Project file
 * Used in callbacks for loading and saving project files, and in edit callback functions
 * Project save data is part of plugin data
 */
struct aviutl2_project_file {
  /**
   * Get string (UTF-8) saved in project
   * @param key Key name (UTF-8)
   * @return Pointer to obtained string (NULL if not set).
   *         Valid until callback processing ends
   */
  char const *(*get_param_string)(char const *key);

  /**
   * Save string (UTF-8) to project
   * @param key Key name (UTF-8)
   * @param value String to save (UTF-8)
   */
  void (*set_param_string)(char const *key, char const *value);

  /**
   * Get binary data saved in project
   * @param key Key name (UTF-8)
   * @param data Pointer to storage for obtained data
   * @param size Size of data to obtain (fails if different from saved size)
   * @return true if data obtained correctly
   */
  bool (*get_param_binary)(char const *key, void *data, int size);

  /**
   * Save binary data to project
   * @param key Key name (UTF-8)
   * @param data Pointer to data to save
   * @param size Size of data to save (4096 bytes or less)
   */
  void (*set_param_binary)(char const *key, void *data, int size);

  /**
   * Delete all data saved in project
   */
  void (*clear_params)(void);

  /**
   * Get project file path
   * @return Pointer to project file path (may not be set)
   *         Valid until callback processing ends
   */
  wchar_t const *(*get_project_file_path)(void);
};

//--------------------------------

/**
 * Host application table
 */
struct aviutl2_host_app_table {
  /**
   * Set plugin information
   * @param information Plugin information
   */
  void (*set_plugin_information)(wchar_t const *information);

  /**
   * Register input plugin
   * @param input_plugin_table Input plugin table
   */
  void (*register_input_plugin)(struct aviutl2_input_plugin_table *input_plugin_table);

  /**
   * Register output plugin
   * @param output_plugin_table Output plugin table
   */
  void (*register_output_plugin)(struct aviutl2_output_plugin_table *output_plugin_table);

  /**
   * Register filter plugin
   * @param filter_plugin_table Filter plugin table
   */
  void (*register_filter_plugin)(struct aviutl2_filter_plugin_table *filter_plugin_table);

  /**
   * Register script module
   * @param script_module_table Script module table
   */
  void (*register_script_module)(struct aviutl2_script_module_table *script_module_table);

  /**
   * Register import menu
   * @param name Import menu name
   * @param func_proc_import Callback function for import menu selection
   */
  void (*register_import_menu)(wchar_t const *name, void (*func_proc_import)(struct aviutl2_edit_section *edit));

  /**
   * Register export menu
   * @param name Export menu name
   * @param func_proc_export Callback function for export menu selection
   */
  void (*register_export_menu)(wchar_t const *name, void (*func_proc_export)(struct aviutl2_edit_section *edit));

  /**
   * Register window client
   * @param name Window name
   * @param hwnd Window handle. WS_CHILD will be added to window and parent window will be set (WS_POPUP will be
   * removed)
   */
  void (*register_window_client)(wchar_t const *name, HWND hwnd);

  /**
   * Get handle for editing project data
   * @return Edit handle
   */
  struct aviutl2_edit_handle *(*create_edit_handle)(void);

  /**
   * Register function to be called when project file is loaded (called during project initialization)
   * @param func_project_load Callback function for project file load
   */
  void (*register_project_load_handler)(void (*func_project_load)(struct aviutl2_project_file *project));

  /**
   * Register function to be called just before project file is saved
   * @param func_project_save Callback function for project file save
   */
  void (*register_project_save_handler)(void (*func_project_save)(struct aviutl2_project_file *project));

  /**
   * Register layer context menu item (layer editor, no selection state)
   * @param name Menu item name
   * @param func_proc_layer_menu Callback invoked on menu selection
   */
  void (*register_layer_menu)(wchar_t const *name, void (*func_proc_layer_menu)(struct aviutl2_edit_section *edit));

  /**
   * Register object context menu item (layer editor, object selected state)
   * @param name Menu item name
   * @param func_proc_object_menu Callback invoked on menu selection
   */
  void (*register_object_menu)(wchar_t const *name, void (*func_proc_object_menu)(struct aviutl2_edit_section *edit));

  /**
   * Register config menu
   * After registering config menu, registering window client will add "Settings" to system menu
   * @param name Config menu name
   * @param func_config Callback function for config menu selection
   */
  void (*register_config_menu)(wchar_t const *name, void (*func_config)(HWND hwnd, HINSTANCE dll_hinst));

  /**
   * Register edit menu
   * @param name Edit menu name. Use '\' in name to create hierarchical display
   * @param func_proc_edit_menu Callback function for edit menu selection
   */
  void (*register_edit_menu)(wchar_t const *name, void (*func_proc_edit_menu)(struct aviutl2_edit_section *edit));

  /**
   * Register function to be called when cache clear operation is performed
   * @param func_proc_clear_cache Callback function for cache clear
   */
  void (*register_clear_cache_handler)(void (*func_proc_clear_cache)(struct aviutl2_edit_section *edit));

  /**
   * Register function to be called right after scene is changed
   * Also called when scene settings are updated
   * @param func_proc_change_scene Callback function for scene change
   */
  void (*register_change_scene_handler)(void (*func_proc_change_scene)(struct aviutl2_edit_section *edit));
};
