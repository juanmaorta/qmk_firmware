#include QMK_KEYBOARD_H

enum custom_layers {
 _BASE,
 _DEV,
 _NAV
};

enum custom_keycodes {
  FUNCTION_START = SAFE_RANGE,
  AMP,
  ASTERIX,
  AT,
  CIRC,
  DBL_QUOTE,
  DOG,
  DOLLAR,
  EQUAL_PLUS,
  GREATER,
  HASH,
  LCURLY,
  LESSER,
  LPARENS,
  LSQUARE_CURLY,
  MIN_SUP,
  PERCENT,
  PIPE,
  RCURLY,
  RPARENS,
  RSQUARE_CURLY,
  SLASH_QUEST,
};

enum unicode_names {
    CKC_SLASH,
    CKC_QUESTION,
};

const uint32_t PROGMEM unicode_map[] = {
    [CKC_SLASH]  = 0x1F4A9,  // /
    [CKC_QUESTION] = 0x20AC,  // ?
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_BASE] = LAYOUT_5x6(
      QK_GESC, KC_1, KC_2, KC_3, KC_4, KC_5,                  KC_6, KC_7, KC_8,    KC_9,   KC_0,    KC_BSPC, 
      KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T,                   KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_MINS, 
      OSM(MOD_LSFT), KC_A, KC_S, KC_D, KC_F, KC_G,            KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT, 
      KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_B,                  KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_BSLS, 
                  KC_LBRC, KC_RBRC,                                       KC_PLUS, KC_EQL, 
      
      KC_SPC, SGUI(KC_A), KC_LALT, KC_SPC, KC_LGUI, MO(2),    MO(1), KC_ENT, KC_NO, LAG(KC_I), SGUI(KC_4), KC_NO),
	[_DEV] = LAYOUT_5x6(
      KC_TRNS, PIPE,  AT,    HASH,  DOLLAR, PERCENT,                CIRC,  AMP,     ASTERIX, LPARENS, RPARENS, DOG, 
      KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO,  FUNCTION_START,         KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, 
      KC_TRNS, KC_1, KC_2, KC_3, KC_4, KC_5,                        KC_6,  KC_7,    KC_8,    KC_9,    KC_0,    DBL_QUOTE, 
      KC_TRNS, KC_NO, LCURLY, RCURLY, KC_NO, KC_NO,                        KC_NO, KC_NO,   LESSER,  GREATER, SLASH_QUEST,   KC_NO, 
                   LSQUARE_CURLY, RSQUARE_CURLY,                               KC_NO,   EQUAL_PLUS, 
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
	[_NAV] = LAYOUT_5x6(
      KC_TRNS, PIPE, AT, HASH, DOLLAR, PERCENT,                CIRC,    AMP,     ASTERIX, LPARENS,   RPARENS, KC_DEL, 
      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_UP,   KC_TRNS,   KC_VOLD, KC_VOLU, 
      KC_TRNS, KC_TRNS, SGUI(KC_S), KC_TRNS, KC_TRNS, KC_TRNS,    KC_PGUP, KC_LEFT, KC_DOWN, KC_RGHT,   KC_RGHT, DBL_QUOTE, 
      KC_TRNS, SGUI(KC_Z), SGUI(KC_X), SGUI(KC_C), SGUI(KC_V), KC_TRNS,    KC_PGDN, KC_TRNS, LESSER,  GREATER,   SLASH_QUEST,   KC_NO,
                        LSQUARE_CURLY, RSQUARE_CURLY,                                        KC_TRNS, EQUAL_PLUS, 
      KC_TRNS, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};

uint8_t mod_state;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // Store the current modifier state in the variable for later reference
  mod_state = get_mods();
  switch(keycode) {

  // Issues a Delete action if backspace is pressed with left shifg
  case KC_BSPC:
        {
        // Initialize a boolean variable that keeps track
        // of the delete key status: registered or not?
        static bool delkey_registered;
        if (record->event.pressed) {
            // Detect the activation of either shift keys
            if (mod_state & MOD_MASK_SHIFT) {
                // First temporarily canceling both shifts so that
                // shift isn't applied to the KC_DEL keycode
                del_mods(MOD_MASK_SHIFT);
                register_code(KC_DEL);
                // Update the boolean variable to reflect the status of KC_DEL
                delkey_registered = true;
                // Reapplying modifier state so that the held shift key(s)
                // still work even after having tapped the Backspace/Delete key.
                set_mods(mod_state);
                return false;
            }
        } else { // on release of KC_BSPC
            // In case KC_DEL is still being sent even after the release of KC_BSPC
            if (delkey_registered) {
                unregister_code(KC_DEL);
                delkey_registered = false;
                return false;
            }
        }
        // Let QMK process the KC_BSPC keycode as usual outside of shift
        return true;
    }
    case PERCENT:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT("5"));
      }
      break;
    case DOG:
      if (record->event.pressed) {
        SEND_STRING("--------");
        SEND_STRING(SS_LSFT("6"));
      }
      break;    
    case FUNCTION_START:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL(" "));
        _delay_ms(50);
        SEND_STRING("() => {}");
        _delay_ms(50);
        SEND_STRING(SS_LCTRL(" "));
      }
      break;
    case PIPE:
      if (record->event.pressed) {
        SEND_STRING(SS_LALT("1"));
      }
      break;    
    case SLASH_QUEST:
      if (record->event.pressed) {
        if (mod_state & MOD_MASK_SHIFT) {
          clear_mods();
          // SEND_STRING(SS_LSFT("'"));
        } else {
          SEND_STRING(SS_LSFT("7"));
        }
      }
      break;
    case AT:
      if (record->event.pressed) {
        SEND_STRING(SS_LALT("2"));
      }
      break;
    case HASH:
      if (record->event.pressed) {
        SEND_STRING(SS_LALT("3"));
      }
      break;    
    case DOLLAR:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT("4"));
      }
      break;
    case AMP:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT("6"));
      }
      break;      
    case CIRC:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT("\1\1\x2F")); // don't know why, but it works
      }
      break;       
    case ASTERIX:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT("]"));
      }
      break;    
    case LESSER:
      if (record->event.pressed) {
        SEND_STRING("`");
      }
      break;    
    case GREATER:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT("`"));
      }
      break;
    case LPARENS:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT("8"));
      }
      break;    
    case RPARENS:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT("9"));
      }
      break;        
    case EQUAL_PLUS:
      if (record->event.pressed) {
        if (mod_state & MOD_MASK_SHIFT) {
          SEND_STRING("+");
        } else {
          SEND_STRING(SS_LSFT("0"));
        }
      }
      break;    
    case LSQUARE_CURLY:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL(" "));
        _delay_ms(50);
        if (mod_state & MOD_MASK_SHIFT) {
          SEND_STRING(SS_LSFT("["));
        } else {
          SEND_STRING("[");
        }
        _delay_ms(50);
        SEND_STRING(SS_LCTRL(" "));
      }
      break;   
    case LCURLY:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL(" "));
        _delay_ms(50);
        SEND_STRING(SS_LSFT("["));
        _delay_ms(50);
        SEND_STRING(SS_LCTRL(" "));
      }
      break;    
    case RSQUARE_CURLY:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL(" "));
        _delay_ms(50);
        if (mod_state & MOD_MASK_SHIFT) {
          SEND_STRING(SS_LSFT("]"));
        } else {
          SEND_STRING("]");
        }
        _delay_ms(50);
        SEND_STRING(SS_LCTRL(" "));
      }
      break;
    case RCURLY:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL(" "));
        _delay_ms(50);
        SEND_STRING(SS_LSFT("]"));
        _delay_ms(50);
        SEND_STRING(SS_LCTRL(" "));
      }
      break;
    case DBL_QUOTE:
      if (record->event.pressed) {
        SEND_STRING(SS_LSFT("2"));
      }
      break;
  }
  return true;
}

