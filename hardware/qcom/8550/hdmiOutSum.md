# 问题
1. 配置hdmi out输出口，未有hdmi log打印，并且hdmi未成功

2. 关键log
1043:340 E qcom-spmi-gpio c42d000.qcom,spmi: qcom,pm8550@1:pinctrl@8800: pin gpio11 already requested by soc:gpio_key; cannot claim for soc:display_gpio_regulator@1
1057:351 W msm_ext_display: loading out-of-tree module taints kernel.
1066:424 I userspace-cdev soc: qcom,userspace-cdev: cdev:display-fps lvl:16 registered
1074:340 E qcom-spmi-gpio c42d000.qcom,spmi: qcom,pm8550@1:pinctrl@8800: pin-10 (soc:display_gpio_regulator@1) status -22
1089:340 E qcom-spmi-gpio c42d000.qcom,spmi: qcom,pm8550@1:pinctrl@8800: could not request pin 10 (gpio11) from group gpio11  on device c42d000.qcom,spmi:qcom,pm8550@1:pinctrl@8800


2179:776 I [drm:dsi_panel_get_mode [msm_drm]] [msm-dsi-info]: default topology: lm: 2 comp_enc:0 intf: 2
2180:776 E         : [drm:dp_parser_pinctrl][msm-dp-err][776 ]failed to get pinctrl active state, rc=-19
2181:776 E         : [drm:dp_init_sub_modules][msm-dp-err][776 ]device tree parsing failed
2183:776 E         : [drm:_sde_kms_setup_displays:2063] [sde error]dp 0 connector init failed

3. 关键字
drm|dsi|dis|dp_

4. 问题代码

	display_panel_avdd: display_gpio_regulator@1 {
		compatible = "qti-regulator-fixed";
		regulator-name = "display_panel_avdd";
		regulator-min-microvolt = <5500000>;
		regulator-max-microvolt = <5500000>;
		regulator-enable-ramp-delay = <233>;
        // 配置问题所在！
		gpio = <&pm8550_gpios 11 0>;
		enable-active-high;
		regulator-boot-on;
		proxy-supply = <&display_panel_avdd>;
		qcom,proxy-consumer-enable;
		pinctrl-names = "default";
		pinctrl-0 = <&display_panel_avdd_default>;
	};
