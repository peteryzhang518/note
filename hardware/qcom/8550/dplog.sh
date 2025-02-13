adb root
adb remount 
adb shell setenforce 0
adb shell mount -t debugfs none /sys/kernel/debug/
adb shell 'echo -n "file dp_display.c +p" > /d/dynamic_debug/control'
adb shell 'echo -n "file dp_drm.c +p" > /d/dynamic_debug/control'
adb shell 'echo -n "file dp_link.c +p" > /d/dynamic_debug/control'
adb shell 'echo -n "file dp_ctrl.c +p" > /d/dynamic_debug/control'
adb shell 'echo -n "file dp_catalog.c +p" > /d/dynamic_debug/control'
adb shell 'echo -n "file dp_usbpd.c +p" > /d/dynamic_debug/control'
adb shell 'echo -n "file dp_aux.c +p" > /d/dynamic_debug/control'
adb shell 'echo -n "file dp_panel.c +p" > /d/dynamic_debug/control'
adb shell 'echo -n "file dp_audio.c +p" > /d/dynamic_debug/control'
