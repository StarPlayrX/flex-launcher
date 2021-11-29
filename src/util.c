#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifdef __unix__
#include <platform/unix.h>
#endif
#ifdef _WIN32
#include <platform/windows.h>
#endif
#include <ini.h>
#include <SDL.h>
#include <launcher.h>
#include <util.h>

menu_t *menu = NULL;
entry_t *entry = NULL;
gamepad_control_t *current_gamepad_control = NULL;

// A function to handle config file parsing
int config_handler(void *user, const char *section, const char *name, const char *value)
{
  config_t* pconfig = (config_t*) user;

  // Parse settings
  if (!strcmp(section,"Settings")) {
    if (!strcmp(name,SETTING_BACKGROUND_IMAGE)) {
      copy_string(value, &pconfig->background_image);
    }
    else if (!strcmp(name,SETTING_TITLE_FONT)) {
      copy_string(value, &pconfig->title_font_path);
    }
    else if (!strcmp(name,SETTING_TITLE_FONT_SIZE)) {
      pconfig->font_size = (unsigned int) atoi(value);
    }
    else if (!strcmp(name,SETTING_TITLE_COLOR)) {
      hex_to_color(value,&pconfig->title_color,32);
    }
    else if (!strcmp(name,SETTING_BACKGROUND_MODE)) {
      if (!strcmp(value,"Image")) {
        pconfig->background_mode = MODE_IMAGE;
      }
      else {
        pconfig->background_mode = MODE_COLOR;
      }
    }
    else if (!strcmp(name,SETTING_BACKGROUND_COLOR)) {
        hex_to_color(value,&pconfig->background_color,24);
    }
    else if (!strcmp(name,SETTING_ICON_SIZE)) {
      Uint16 icon_size = (Uint16) atoi(value);
      if (icon_size >= MIN_ICON_SIZE && icon_size <= MAX_ICON_SIZE) {
        pconfig->icon_size = icon_size;
      }
    }
    else if (!strcmp(name,SETTING_DEFAULT_MENU)) {
      copy_string(value, &pconfig->default_menu);
    }
    else if (!strcmp(name,SETTING_HIGHLIGHT_COLOR)) {
      hex_to_color(value,&pconfig->highlight_color,32);
    }
    else if (!strcmp(name,SETTING_HIGHLIGHT_CORNER_RADIUS)) {
      Uint16 rx = (Uint16) atoi(value);
      if (rx >= MIN_RX_SIZE && rx <= MAX_RX_SIZE) {
        pconfig->highlight_rx = rx;
      }
    }
    else if (!strcmp(name,SETTING_TITLE_PADDING)) {
      int title_padding = atoi(value);
      if (title_padding >= 0) {
        pconfig->title_padding = (unsigned int) title_padding;
      }
    }
    else if (!strcmp(name,SETTING_MAX_BUTTONS)) {
      int max_buttons = atoi(value);
      if (max_buttons > 0) {
        pconfig->max_buttons = (unsigned int) max_buttons;
      }
    }
    else if (!strcmp(name,SETTING_ICON_SPACING)) {
      if (strstr(value,"%") != NULL && strlen(value) < 6) {
        strcpy(pconfig->icon_spacing_str,value);
      }
      else {
        int icon_spacing = atoi(value);
        if (icon_spacing > 0 || !strcmp(value,"0")) {
          pconfig->icon_spacing = icon_spacing;
        }
      }
    }
    else if (!strcmp(name,SETTING_HIGHLIGHT_VPADDING)) {
      int highlight_vpadding = atoi(value);
      if (highlight_vpadding > 0 || !strcmp(value,"0")) {
        pconfig->highlight_vpadding = highlight_vpadding;
      }
    }
    else if (!strcmp(name,SETTING_HIGHLIGHT_HPADDING)) {
      int highlight_hpadding = atoi(value);
      if (highlight_hpadding > 0 || !strcmp(value,"0")) {
        pconfig->highlight_hpadding = highlight_hpadding;
      }
    }
    else if (!strcmp(name,SETTING_TITLE_OPACITY)) {
      if (strstr(value,".") == NULL && 
      strstr(value,"%") != NULL && 
      strlen(value) < PERCENT_MAX_CHARS) {
        strcpy(pconfig->title_opacity,value);
      }
    }
    else if (!strcmp(name,SETTING_HIGHLIGHT_OPACITY)) {
      if (strstr(value,".") == NULL && 
      strstr(value,"%") != NULL && 
      strlen(value) < PERCENT_MAX_CHARS) {
        strcpy(pconfig->highlight_opacity,value);
      }
    }
    else if (!strcmp(name,SETTING_BUTTON_CENTERLINE)) {
      if (strlen(value) < PERCENT_MAX_CHARS) {
        strcpy(pconfig->button_centerline,value);
      }
    }
    else if (!strcmp(name,SETTING_SCROLL_INDICATORS)) {
      pconfig->scroll_indicators = convert_bool(value, DEFAULT_SCROLL_INDICATORS);
    }
    else if (!strcmp(name,SETTING_SCROLL_INDICATOR_COLOR)) {
      hex_to_color(value,&pconfig->scroll_indicator_color,32);
    }
    else if (!strcmp(name,SETTING_SCROLL_INDICATOR_OPACITY)) {
      if (strstr(value,".") == NULL && 
      strstr(value,"%") != NULL && 
      strlen(value) < PERCENT_MAX_CHARS) {
        strcpy(pconfig->scroll_indicator_opacity,value);
      }
    }
    else if (!strcmp(name,SETTING_TITLE_OVERSIZE_MODE)) {
      if (!strcmp(value,"Shrink")) {
        pconfig->title_oversize_mode = MODE_TEXT_SHRINK;
      }
      else if (!strcmp(value,"None")) {
        pconfig->title_oversize_mode = MODE_TEXT_NONE;
      }
    }
    else if (!strcmp(name,SETTING_RESET_ON_BACK)) {
      pconfig->reset_on_back = convert_bool(value, DEFAULT_RESET_ON_BACK);
    }
    else if (!strcmp(name,SETTING_ESC_QUIT)) {
      pconfig->esc_quit = convert_bool(value, DEFAULT_ESC_QUIT);
    }
  }

  // Parse gamepad settings
  else if (!strcmp(section, "Gamepad")) {
    if (!strcmp(name, SETTING_GAMEPAD_ENABLED)) {
      pconfig->gamepad_enabled = convert_bool(value, DEFAULT_GAMEPAD_ENABLED);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_DEVICE)) {
      int device_index = atoi(value);
      if (device_index >= 0) {
        pconfig->gamepad_device = device_index;
      }
    }
    else if (!strcmp(name, SETTING_GAMEPAD_MAPPINGS_FILE)) {
      copy_string(value, &pconfig->gamepad_mappings_file);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_LSTICK_XM)) {
      add_gamepad_control(TYPE_AXIS_NEG, SDL_CONTROLLER_AXIS_LEFTX, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_LSTICK_XP)) {
      add_gamepad_control(TYPE_AXIS_POS, SDL_CONTROLLER_AXIS_LEFTX, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_LSTICK_YM)) {
      add_gamepad_control(TYPE_AXIS_NEG, SDL_CONTROLLER_AXIS_LEFTY, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_LSTICK_YP)) {
      add_gamepad_control(TYPE_AXIS_POS, SDL_CONTROLLER_AXIS_LEFTY, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_RSTICK_XM)) {
      add_gamepad_control(TYPE_AXIS_NEG, SDL_CONTROLLER_AXIS_RIGHTX, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_RSTICK_XP)) {
      add_gamepad_control(TYPE_AXIS_POS, SDL_CONTROLLER_AXIS_RIGHTX, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_RSTICK_YM)) {
      add_gamepad_control(TYPE_AXIS_NEG, SDL_CONTROLLER_AXIS_RIGHTY, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_RSTICK_YP)) {
      add_gamepad_control(TYPE_AXIS_POS, SDL_CONTROLLER_AXIS_RIGHTY, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_LTRIGGER)) {
      add_gamepad_control(TYPE_AXIS_POS, SDL_CONTROLLER_AXIS_TRIGGERLEFT, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_RTRIGGER)) {
      add_gamepad_control(TYPE_AXIS_POS, SDL_CONTROLLER_AXIS_TRIGGERRIGHT, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_A)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_A, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_B)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_B, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_X)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_X, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_Y)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_Y, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_BACK)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_BACK, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_GUIDE)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_GUIDE, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_START)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_START, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_LEFT_STICK)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_LEFTSTICK, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_RIGHT_STICK)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_RIGHTSTICK, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_LEFT_SHOULDER)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_LEFTSHOULDER, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_RIGHT_SHOULDER)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_DPAD_UP)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_DPAD_UP, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_DPAD_DOWN)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_DPAD_DOWN, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_DPAD_LEFT)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_DPAD_LEFT, name, value, pconfig);
    }
    else if (!strcmp(name, SETTING_GAMEPAD_BUTTON_DPAD_RIGHT)) {
      add_gamepad_control(TYPE_BUTTON, SDL_CONTROLLER_BUTTON_DPAD_RIGHT, name, value, pconfig);
    }
  }

  // Parse menus/entries
  else {
    entry_t *previous_entry;

    // Check if menu struct exists for current section
    if (pconfig->first_menu == NULL) {
      pconfig->first_menu = create_menu(section, &pconfig->num_menus);
      menu = pconfig->first_menu;
    }
    else {
      bool menu_exists = false;
      for (menu_t *tmp = pconfig->first_menu; tmp != NULL;
      tmp = tmp->next) {
        if (!strcmp(tmp->name,section)) {
          menu_exists = true;
          break;
        }
      }

    // Create menu if it doesn't already exist
      if (menu_exists == false) {
        menu->next = create_menu(section, &pconfig->num_menus);
        menu = menu->next;
      }
    }

    // Parse entry line for title, icon path, command
    char *string = (char*) value;
    char *token;
    token = strtok(string, ";");
    if (token != NULL) {

      // Create first entry in the menu if none exists
      if (menu->first_entry == NULL) {
        menu->first_entry = malloc(sizeof(entry_t));
        entry = menu->first_entry;
        entry->next = NULL;
      }

      // Add entry to the end of the linked list
      else {
        previous_entry = entry;
        entry = entry->next;
        entry = malloc(sizeof(entry_t));
        previous_entry->next = entry;
        entry->next = NULL;
      }
      entry->title_offset = 0;
    }

    // Store data in entry struct
    int i;
    for (i = 0;i < 3 && token != NULL; i++) {
        if (i == 0) {
          copy_string(token, &entry->title);
        }
        else if (i == 1) {
          copy_string(token, &entry->icon_path);
        }
        else if (i == 2){
          copy_string(token, &entry->cmd);
        }
        token = strtok(NULL, ";");
    }

    // Delete entry if parse failed to find 3 valid tokens
    if (i != 3 || !strcmp(":select", entry->cmd)) {
      if (menu->num_entries == 0) {
        free(menu->first_entry);
        menu->first_entry = NULL;
      }
      else {
        free(entry);
        entry = previous_entry;
        entry->next = NULL;
      }
    }
    else {
      if (menu->num_entries == 0) {
        entry->previous = NULL;
      }
      else {
        entry->previous = previous_entry;
      }
      menu->num_entries++;
    }
  }
  return 0;
}

// A function to convert a hex-formatted string into a color struct
bool hex_to_color(char *text, SDL_Color *color, int bits)
{

  // If strtoul returned 0, and the hex string wasn't 000..., then there was an error
  Uint32 hex = (Uint32) strtoul(text,NULL,16);
  if ((!hex && strcmp(text,"00000000")) || 
  (!hex && strcmp(text,"000000")) || 
  strlen(text) != bits/4) {
    return false;
  }

  // Convert int to SDL_Color struct via bitwise logic
  if (bits == 32) {
    color->r = (Uint8) (hex >> 24);
    color->g = (Uint8) ((hex & 0x00ff0000) >> 16);
    color->b = (Uint8) ((hex & 0x0000ff00) >> 8);
    color->a = (Uint8) (hex & 0x000000ff);
    return true;
  }
  else if (bits == 24) {
    color->r = (Uint8) (hex >> 16);
    color->g = (Uint8) ((hex & 0x0000ff00) >> 8);
    color->b = (Uint8) (hex & 0x000000ff);
    color->a = (Uint8) 0xFF;
    return true;
  }
  else {
    return false;
  }
}

// A function to convert a string into a bool
bool convert_bool(char *string, bool default_setting)
{
  if (!strcmp(string, "true")) {
    return true;
  }
  else if (!strcmp(string, "false")) {
    return false;
  }
  else {
    return default_setting;
  }
}

// Allocates memory and copies a variable length string
void copy_string(char *string, char **ptr)
{
  if (strlen(string)) {
    *ptr = malloc(sizeof(char)*(strlen(string) + 1));
    strcpy(*ptr, string);
  }
  else {
    *ptr = NULL;
  }
}

// A function to join paths together
char *join_path(int num_paths, ...)
{
  va_list list;
  va_start(list, num_paths);
  char *arg;
  char temp_path[MAX_PATH_LENGTH];

  for (int i = 0; i < num_paths; i++) {
    arg = va_arg(list, char*);
    if (i == 0) {
      strcpy(temp_path, arg);
    }
    else {
      if (*arg == '/' || *arg == '\\') {
        strcat(temp_path, arg + 1);
      }
      else {
        strcat(temp_path, arg);
      }
    }

    // Add trailing slash if not present
    if (*(temp_path + strlen(temp_path) - 1) != '/' &&
    *(temp_path + strlen(temp_path) - 1) != '\\')  {
      strcat(temp_path, "/");
    }
  }
  va_end(list);
  char *output_path;
  copy_string(temp_path, &output_path);
  return output_path;
}

// A function to find a file from a filename and list of path prefixes
char *find_file(char *file, int num_prefixes, char **prefixes)
{
  char tmp[MAX_PATH_LENGTH];
  for (int i = 0; i < num_prefixes; i++) {
    if (prefixes[i] != NULL && (strlen(prefixes[i]) +
    strlen(file) + 1) <= MAX_PATH_LENGTH) {
      strcpy(tmp,prefixes[i]);
      strcat(tmp,file);
      if (file_exists(tmp)) {
        char *output;
        copy_string(tmp, &output);
        return output;
      }
    }
  }
  return NULL;
}

// Calculates the length of a utf-8 encoded string
int utf8_length(char *string)
{
  int length = 0;
  char *ptr = string;
  while (*ptr != '\0') {
    // If byte is 0xxxxxxx, then it's a 1 byte (ASCII) char
    if ((*ptr & 0x80) == 0) {
      ptr++;
    }
    // If byte is 110xxxxx, then it's a 2 byte char
    else if ((*ptr & 0xE0) == 0xC0) {
      ptr +=2;
    }
    // If byte is 1110xxxx, then it's a 3 byte char
    else if ((*ptr & 0xF0) == 0xE0) {
      ptr +=3;
    }
    // If byte is 11110xxx, then it's a 4 byte char
    else if ((*ptr & 0xF8) == 0xF0) {
      ptr+=4;    
    }
  length++;
  }
  return length;
}

// A function to truncate a utf-8 encoding string to max number of pixels
void utf8_truncate(char *string, int width, int max_width)
{
  int string_length = utf8_length(string);
  int avg_width = width / string_length;
  int num_chars = max_width / avg_width;
  int spaces = (string_length - num_chars) + 3; // Number of spaces to go back
  char *ptr = string + strlen(string);  // Change to null character of string
  int chars = 0;

  // Go back required number of spaces
  do {
    ptr--;    
    if (!(*ptr & 0x80)) { // ASCII characters have 0 as most significant bit
      chars++;
    }
    else { // Non-ASCII character detected
      do {
        ptr--;
      } while (ptr > string && (*ptr & 0xC0) == 0x80); // Non-ASCII most significant byte begins with 0b11
      chars++;
    }
  } while (chars < spaces);

  // Add "..." to end of string to inform user of truncation
  if (strlen(ptr) > 2) {
    *ptr = '.';
    *(ptr + 1) = '.';
    *(ptr + 2) = '.';
    *(ptr + 3) = '\0';
  }
}

// A function to handle the arguments from the command line
int handle_arguments(int argc, char *argv[], char **config_file_path, config_t *config)
{

  // Parse command line arguments
  if (argc > 1) {
    bool version = false;
    bool help = false;
    int config_file_index = -1;
    for (int i = 1; i < argc; i++) {

      // Current argument is config file path if -c or --config was previous argument
      if (*config_file_path == NULL && i == config_file_index) {
        copy_string(argv[i], config_file_path);
      }
      if (!strcmp(argv[i],"-c") || !strcmp(argv[i],"--config")) {
        config_file_index = i + 1;
      }
      else if (!strcmp(argv[i],"-d") || !strcmp(argv[i],"--debug")) {
        config->debug = true;
      }
      else if (!strcmp(argv[i],"-h") || !strcmp(argv[i],"--help")) {
        help = true;
      }
      else if (!strcmp(argv[i],"-v") || !strcmp(argv[i],"--version")) {
        version = true;
      }
      else if (i != config_file_index) {
        printf("Unrecognized option %s\n",argv[i]);
        print_usage();
        return ERROR_QUIT;
      }
    }

    // Check version, help flags
    if (version) {
      print_version();
      if (*config_file_path == NULL) {
        return NO_ERROR_QUIT;
      }
    }
    if (help) {
      print_usage();
      if (*config_file_path == NULL) {
        return NO_ERROR_QUIT;
      }
    }
    if (*config_file_path != NULL) {
      return NO_ERROR;
    } 
  }

  // Try to find config file if none is specified on the command line
  int num_prefixes = 4;
  char **prefixes = malloc(sizeof(char*)*num_prefixes);
  prefixes[0] = "./";
  prefixes[1] = config->exe_path;
  #ifdef __unix__
  prefixes[2] = join_path(3, getenv("HOME"), ".config", EXECUTABLE_TITLE);
  prefixes[3] = PATH_CONFIG_SYSTEM;
  #else
  prefixes[2] = NULL;
  prefixes[3] = NULL;
  #endif
  *config_file_path = find_file(FILENAME_DEFAULT_CONFIG,num_prefixes,prefixes);
  free(prefixes[2]);
  free(prefixes);
  if (*config_file_path == NULL) {
    printf("Fatal Error: No config file found\n");
    print_usage();
    return ERROR_QUIT;
  }
  else {
    return NO_ERROR;
  }
}

// A function to add a gamepad control to the linked list
void add_gamepad_control(int type, int index, char *label, char *cmd, config_t *config)
{
  if (!strlen(cmd)) {
    return;
  }

  // Begin the linked list if none exists
  if (config->gamepad_controls == NULL) {
    config->gamepad_controls = malloc(sizeof(gamepad_control_t));
    current_gamepad_control = config->gamepad_controls;
  }

  // Add another node to the linked list
  else {
    current_gamepad_control->next = malloc(sizeof(gamepad_control_t));
    current_gamepad_control = current_gamepad_control->next;
  }

  // Copy the parameters in the struct
  current_gamepad_control->type = type;
  current_gamepad_control->index = index;
  current_gamepad_control->repeat = 0;
  copy_string(label, &current_gamepad_control->label);
  copy_string(cmd, &current_gamepad_control->cmd);
  current_gamepad_control->next = NULL;
}

// A function to print usage to the command line
void print_usage()
{
  printf("Usage: %s [OPTIONS]\n",EXECUTABLE_TITLE);
  printf("-c, --config             Path to config file.\n");
  printf("-d, --debug              Enable debug messages.\n");
  printf("-h, --help               Show this help message.\n");
  printf("-v, --version            Print version information.\n");
}

// A function to print the version and other info to command line
void print_version()
{
  #if (PROJECT_VERSION_PATCH + 0)
  printf("%s version %i.%i.%i\n", PROJECT_NAME,
                                  PROJECT_VERSION_MAJOR,
                                  PROJECT_VERSION_MINOR,
                                  PROJECT_VERSION_PATCH);
  #else
  printf("%s version %i.%i\n", PROJECT_NAME, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR);
  #endif
}

// A function to print the parsed settings to the command line
void debug_settings(config_t *config)
{
  printf("======================== Settings ========================\n");
  printf("%s: %s\n",SETTING_DEFAULT_MENU,config->default_menu);
  printf("%s: %i\n",SETTING_MAX_BUTTONS,config->max_buttons);
  if (config->background_mode == MODE_COLOR) {
    printf("%s: %s\n",SETTING_BACKGROUND_MODE,"Color");
  }
  else if (config->background_mode == MODE_IMAGE) {
    printf("%s: %s\n",SETTING_BACKGROUND_MODE,"Image");
  }
  printf("%s R: %i\n",SETTING_BACKGROUND_COLOR,config->background_color.r);
  printf("%s G: %i\n",SETTING_BACKGROUND_COLOR,config->background_color.g);
  printf("%s B: %i\n",SETTING_BACKGROUND_COLOR,config->background_color.b);
  if (config->background_image != NULL) {
    printf("%s: %s\n",SETTING_BACKGROUND_IMAGE,config->background_image);
  }
  printf("%s: %i\n",SETTING_ICON_SIZE,config->icon_size);
  printf("%s: %i\n",SETTING_ICON_SPACING,config->icon_spacing);
  printf("%s: %s\n",SETTING_TITLE_FONT,config->title_font_path);
  printf("%s: %i\n",SETTING_TITLE_FONT_SIZE,config->font_size);
  printf("%s R: %i\n",SETTING_TITLE_COLOR,config->title_color.r);
  printf("%s G: %i\n",SETTING_TITLE_COLOR,config->title_color.g);
  printf("%s B: %i\n",SETTING_TITLE_COLOR,config->title_color.b);
  printf("%s A: %i\n",SETTING_TITLE_COLOR,config->title_color.a);
  if (config->title_oversize_mode == MODE_TEXT_TRUNCATE) {
    printf("%s: %s\n",SETTING_TITLE_OVERSIZE_MODE,"Truncate");
  }
  else if (config->title_oversize_mode == MODE_TEXT_SHRINK) {
    printf("%s: %s\n",SETTING_TITLE_OVERSIZE_MODE,"Shrink");
  }
  else if (config->title_oversize_mode == MODE_TEXT_NONE) {
    printf("%s: %s\n",SETTING_TITLE_OVERSIZE_MODE,"None");
  }
  printf("%s: %i\n",SETTING_TITLE_PADDING,config->title_padding);
  printf("%s R: %i\n",SETTING_HIGHLIGHT_COLOR,config->highlight_color.r);
  printf("%s G: %i\n",SETTING_HIGHLIGHT_COLOR,config->highlight_color.g);
  printf("%s B: %i\n",SETTING_HIGHLIGHT_COLOR,config->highlight_color.b);
  printf("%s A: %i\n",SETTING_HIGHLIGHT_COLOR,config->highlight_color.a);
  printf("%s: %i\n",SETTING_HIGHLIGHT_CORNER_RADIUS,config->highlight_rx);
  printf("%s: %i\n",SETTING_HIGHLIGHT_VPADDING,config->highlight_vpadding);
  printf("%s: %i\n",SETTING_HIGHLIGHT_HPADDING,config->highlight_hpadding);
  if (config->scroll_indicators) {
    printf("%s: %s\n",SETTING_SCROLL_INDICATORS,"true");
  }
  else {
    printf("%s: %s\n",SETTING_SCROLL_INDICATORS,"false");
  }
  printf("%s R: %i\n",SETTING_SCROLL_INDICATOR_COLOR,config->highlight_color.r);
  printf("%s G: %i\n",SETTING_SCROLL_INDICATOR_COLOR,config->highlight_color.g);
  printf("%s B: %i\n",SETTING_SCROLL_INDICATOR_COLOR,config->highlight_color.b);
  printf("%s A: %i\n",SETTING_SCROLL_INDICATOR_COLOR,config->highlight_color.a);
  if (config->reset_on_back) {
    printf("%s: %s\n",SETTING_RESET_ON_BACK,"true");
  }
  else {
    printf("%s: %s\n",SETTING_RESET_ON_BACK,"false");
  }
  if (config->esc_quit) {
    printf("%s: %s\n",SETTING_ESC_QUIT,"true");
  }
  else {
    printf("%s: %s\n",SETTING_ESC_QUIT,"false");
  }
  printf("======================== Gamepad ========================\n");
  if (config->gamepad_enabled) {
    printf("%s: %s\n",SETTING_GAMEPAD_ENABLED, "true");
  }
  else {
    printf("%s: %s\n",SETTING_GAMEPAD_ENABLED, "false");
  }
  printf("%s: %i\n", SETTING_GAMEPAD_DEVICE, config->gamepad_device);
  if (config->gamepad_mappings_file != NULL) {
    printf("%s: %s\n", SETTING_GAMEPAD_MAPPINGS_FILE, config->gamepad_mappings_file);
  }
  for (gamepad_control_t *i = config->gamepad_controls; i != NULL; i = i->next) {
    printf("%s: %s\n", i->label, i->cmd);
  }
}

// A function to print the parsed menu entries to the command line
void debug_menu_entries(menu_t *first_menu, int num_menus) 
{
  if (first_menu == NULL) {
    printf("No valid menus found\n");
    return;
  }
  printf("====================== Menu Entries ======================\n");
  menu = first_menu;
  for (int i = 0; i < num_menus; i ++) {
    printf("Menu Name: %s\n",menu->name);
    printf("Number of Entries: %i\n",menu->num_entries);
    entry = menu->first_entry;
    for (int j = 0; j < menu->num_entries; j++) {
      printf("Entry %i Title: %s\n",j,entry->title);
      printf("Entry %i Icon Path: %s\n",j,entry->icon_path);
      printf("Entry %i Command: %s\n",j,entry->cmd);
      if (j != menu->num_entries - 1) {
        printf("\n");
      }
      entry = entry->next;
    }
    if (i != num_menus - 1) {
      printf("----------------------------------------------------------\n");
    }
    menu = menu->next;
  }
}

// A function to print the current button geometry to the command line
void debug_button_positions(entry_t *entry, menu_t *current_menu, geometry_t *geo)
{
  printf("================= Menu \"%s\" Page %i =================\n",
         current_menu->name,
         current_menu->page);
  int left_margin;
  int right_margin;
  for (int i = 0; i < geo->num_buttons; i++) {
    if (i == 0) {
      left_margin = entry->icon_rect.x;
    }
    else if (i == geo->num_buttons - 1) {
      right_margin = geo->screen_width - (entry->icon_rect.x + entry->icon_rect.w);
    }
    printf("Button %i:\n",i);
    printf("Icon X1: %i\n",entry->icon_rect.x);
    printf("Icon X2: %i\n",entry->icon_rect.x + entry->icon_rect.w);
    printf("Icon Y1: %i\n",entry->icon_rect.y);
    printf("Icon Y2: %i\n",entry->icon_rect.y + entry->icon_rect.h);
    printf("Text X1: %i\n",entry->text_rect.x);
    printf("Text X2: %i\n",entry->text_rect.x + entry->icon_rect.w);
    printf("Text Y1: %i\n",entry->text_rect.y);
    printf("Text Y2: %i\n\n",entry->text_rect.y + entry->icon_rect.h);
    entry = entry->next;
  }
  printf("Total Buttons: %i\n",geo->num_buttons);
  printf("Left Margin: %i\n",left_margin);
  printf("Right Margin: %i\n",right_margin);
}

// A function to convert a percentage string to a integer value
int convert_percent(char *string, int max_value)
{
  float percent = atof(strtok(string,"%"));
  if (percent < 0 || percent > 100) {
    return -1;
  }
  else {
    return (int)((float) max_value*percent*0.01F);
  }
}

// A function to retreive menu struct from the linked list via the menu name
menu_t *get_menu(char *menu_name, menu_t *first_menu)
{
  for (menu_t *menu = first_menu; menu != NULL; menu = menu->next) {
    if (!strcmp(menu_name, menu->name)) {
      return menu;
    }
  }
  return NULL;
}

// A function to allocate memory to and initialize a menu struct
menu_t *create_menu(char *menu_name, int *num_menus)
{
  menu_t *menu = malloc(sizeof(menu_t));
  copy_string(menu_name, &menu->name);  
  menu->first_entry = NULL;
  menu->next = NULL;
  menu->back = NULL;
  menu->root_entry = NULL;
  menu->num_entries = 0;
  menu->page = 0;
  menu->highlight_position = 0;
  menu->rendered = false;
  (*num_menus)++;
  return menu;
}