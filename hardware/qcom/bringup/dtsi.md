bring up基本流程
1. 配置dtsi
2. 修改sensor xml
3. 修改sensor module xmk
4. 修改tools/buildbins/buildbins.yaml 另其在被调用python脚本时可以编译出bin
5. 配置oem/qcom/buildbins/build/android/binary_kalama/com.qti.sensormodule.boe0_imx586.mk 
文件，使其可以调用python等编译出bin文件
5. 修改build/infrastructure/camera-techpack/picklist/picklist_camera.yaml  
6. 配置chi configs/product.mk  
common config/device-vendor.mk
common config/device-vendor-qssi.mk
camx build/infrastructure/camera-techpack/picklist/picklist_camera.yaml
使用生成的bin文件
7. 配置刚完成的相机的id oem/qcom/multicamera/chimcxcameraconfig/configs/kailua/kailua_supersetdc.xml
8. 把配置id文件的配置加入到camx的公共配置中camx-common/settings/sm8750/camxsettings.xml


结合图片来看，系统先用sensor xml probe 成功了之后去oem/qcom/multicamera/config/..找到了config
枚举了相机之后再: Loaded "QTI Camera HAL" camera module ，  应该是再去使用camera module 查找能力






代码流程
在图片上的probe success 之后
1. hal会PickMatchingCameraXML  成功了之后去oem/qcom/multicamera/config/..找到了config
2. EnumerateGeneratedCameras
3. GetNumCameras(
4. ] camxhal3.cpp:2853 get_concurrent_streaming_camera_ids() 












      <setting>
            <Name>Use Logical Camera XML</Name>
            <Help>Use this xml file when building the logical cam info. This must exist under
                  chi-cdk/oem/qcom/multicamera/logical_cam_info/TARGET_BOARD_PLATFORM_</Help>
            <VariableName>multiCameraLogicalXMLFile</VariableName>
            <VariableType>String</VariableType>
            <SetpropKey>persist.vendor.camera.multicam.multicameralogicalxmlfile</SetpropKey>
            <!-- Please note that an underscore means use slotId/sensorName matching -->
            <DefaultValue>kailua_supersetdc.xml</DefaultValue>
            <Dynamic>FALSE</Dynamic>
        </setting>

&cam_cci0 {

	actuator_triple_wide: qcom,actuator2 {
		cell-index = <2>;
		compatible = "qcom,actuator";
		cci-master = <CCI_MASTER_1>;
		cam_vaf-supply = <&L7M>;
		regulator-names = "cam_vaf";
		rgltr-cntrl-support;
		rgltr-min-voltage = <2960000>;
		rgltr-max-voltage = <2960000>;
		rgltr-load-current = <119000>;
		status = "disabled";
	};

	actuator_rear_aux: qcom,actuator8 {
		cell-index = <8>;
		compatible = "qcom,actuator";
		cci-master = <CCI_MASTER_0>;
		cam_vaf-supply = <&L7M>;
		regulator-names = "cam_vaf";
		rgltr-cntrl-support;
		rgltr-min-voltage = <2960000>;
		rgltr-max-voltage = <2960000>;
		rgltr-load-current = <100000>;
		status = "disabled";
	};

	ois_wide: qcom,ois0 {
		cell-index = <2>;
		compatible = "qcom,ois";
		cci-master = <CCI_MASTER_1>;
		cam_vio-supply = <&L5M>;
		cam_vaf-supply = <&L7M>;
		cam_clk-supply = <&cam_cc_titan_top_gdsc>;
		regulator-names = "cam_vio","cam_vaf", "cam_clk";
		rgltr-cntrl-support;
		rgltr-min-voltage = <1800000 2960000 0>;
		rgltr-max-voltage = <1800000 2960000 0>;
		rgltr-load-current = <4000 220000 0>;
		status = "disabled";
	};

	eeprom_tof1: qcom,eeprom7 {
		cell-index = <7>;
		compatible = "qcom,eeprom";
		cam_vio-supply = <&L5M>;
		cam_vdig-supply = <&S7I>;
		cam_clk-supply = <&cam_cc_titan_top_gdsc>;
		cam_vana-supply = <&L5N>;
		cam_v_custom1-supply = <&L7N>;
		cam_v_custom2-supply = <&DBO3>;
		regulator-names = "cam_vio", "cam_vdig",
			"cam_clk", "cam_vana", "cam_v_custom1", "cam_v_custom2";
		rgltr-cntrl-support;
		rgltr-min-voltage = <1800000 1256000 0 2800000 3304000 3600000>;
		rgltr-max-voltage = <1800000 1340000 0 2800000 3304000 3600000>;
		rgltr-load-current = <155000 680000 0 50000 30000 2500000>;
		gpio-no-mux = <0>;
		pinctrl-names = "cam_default", "cam_suspend";
		pinctrl-0 = <&cam_sensor_mclk0_active
			&cam_sensor_active_rst0>;
		pinctrl-1 = <&cam_sensor_mclk0_suspend
			&cam_sensor_suspend_rst0>;
		gpios = <&tlmm 89 0>,
			<&tlmm 13 0>;
		gpio-reset = <1>;
		gpio-req-tbl-num = <0 1>;
		gpio-req-tbl-flags = <1 0>;
		gpio-req-tbl-label = "CAMIF_MCLK0",
					"CAM_RESET0";
		cci-master = <CCI_MASTER_0>;
		clocks = <&cambistmclkcc CAM_BIST_MCLK_CC_MCLK0_CLK>;
		clock-names = "cam_clk";
		clock-cntl-level = "nominal";
		clock-rates = <24000000>;
		status = "disabled";
	};

	eeprom_rear_aux: qcom,eeprom8 {
		cell-index = <8>;
		compatible = "qcom,eeprom";
		cam_vio-supply = <&L5M>;
		cam_vdig-supply = <&L1N>;
		cam_clk-supply = <&cam_cc_titan_top_gdsc>;
		cam_vana-supply = <&L3M>;
		cam_vaf-supply = <&L7M>;
		cam_v_custom1-supply = <&L6M>;
		regulator-names = "cam_vio", "cam_vdig", "cam_clk", "cam_vana",
			"cam_vaf", "cam_v_custom1";
		rgltr-cntrl-support;
		rgltr-min-voltage = <1800000 1104000 0 2800000 2960000 2800000>;
		rgltr-max-voltage = <1800000 1104000 0 2800000 2960000 2800000>;
		rgltr-load-current = <6400 850000 0 114500 100000 140000>;
		gpio-no-mux = <0>;
		pinctrl-names = "cam_default", "cam_suspend";
		pinctrl-0 = <&cam_sensor_mclk0_active
				&cam_sensor_active_rst0>;
		pinctrl-1 = <&cam_sensor_mclk0_suspend
				&cam_sensor_suspend_rst0>;
		gpios = <&tlmm 89 0>,
			<&tlmm 13 0>;
		gpio-reset = <1>;
		gpio-req-tbl-num = <0 1>;
		gpio-req-tbl-flags = <1 0>;
		gpio-req-tbl-label = "CAMIF_MCLK0",
					"CAM_RESET0";
		cci-master = <CCI_MASTER_0>;
		clocks = <&cambistmclkcc CAM_BIST_MCLK_CC_MCLK0_CLK>;
		clock-names = "cam_clk";
		clock-cntl-level = "nominal";
		clock-rates = <24000000>;
		status = "disabled";
	};

	eeprom_wide: qcom,eeprom2 {
		cell-index = <2>;
		compatible = "qcom,eeprom";
		cam_vio-supply = <&L5M>;
		cam_vdig-supply = <&L1N>;
		cam_clk-supply = <&cam_cc_titan_top_gdsc>;
		cam_vana-supply = <&L6M>;
		cam_vaf-supply = <&L7M>;
		cam_v_custom1-supply = <&L3N>;
		regulator-names = "cam_vio", "cam_vdig", "cam_clk", "cam_vana",
			"cam_vaf", "cam_v_custom1";
		rgltr-cntrl-support;
		rgltr-min-voltage = <1800000 1104000 0 2800000 2960000 1800000>;
		rgltr-max-voltage = <1800000 1104000 0 2800000 2960000 1800000>;
		rgltr-load-current = <4000 913000 0 91400 119000 63100>;
		gpio-no-mux = <0>;
		pinctrl-names = "cam_default", "cam_suspend";
		pinctrl-0 = <&cam_sensor_mclk1_active
				&cam_sensor_active_rst1>;
		pinctrl-1 = <&cam_sensor_mclk1_suspend
				&cam_sensor_suspend_rst1>;
		gpios = <&tlmm 90 0>,
			<&tlmm 15 0>;
		gpio-reset = <1>;
		gpio-req-tbl-num = <0 1>;
		gpio-req-tbl-flags = <1 0>;
		gpio-req-tbl-label = "CAMIF_MCLK1",
					"CAM_RESET1";
		cci-master = <CCI_MASTER_1>;
		clocks = <&cambistmclkcc CAM_BIST_MCLK_CC_MCLK1_CLK>;
		clock-names = "cam_clk";
		clock-cntl-level = "nominal";
		clock-rates = <24000000>;
		status = "disabled";
	};
	qcom,cam-sensor0 {
        //pz 配置标识的索引，需要配置的和sensor module文件中的值一样
		cell-index = <0>;
        //属性是什么
		compatible = "qcom,cam-sensor";
        //
		csiphy-sd-index = <0>;
        //
		?eeprom-src = <&eeprom_tof1>;
        //sensor 的滚转角度等值
		sensor-position-roll = <270>;
		sensor-position-pitch = <0>;
		sensor-position-yaw = <180>;
        //sensor 使用的电 各种不同的电 哪根线，哪根引脚
		cam_vio-supply = <&camera2_vio_ldo>;
		cam_clk-supply = <&cam_cc_titan_top_gdsc>;
        //sensor 使用的电 各种不同的电 名字总
		regulator-names = "cam_vio", "cam_clk";
		rgltr-cntrl-support;
        //sensor 使用的电 各种不同的电 值
		rgltr-min-voltage = <1800000 0>;
		rgltr-max-voltage = <1800000 0>;
		rgltr-load-current = <4000 0>;

        //gpio 支持复用功能
		gpio-no-mux = <0>;
        //gpio 引脚的模式
		pinctrl-names = "cam_default", "cam_suspend";
        //gpio 引脚第一个模式的配置
        pinctrl-0 = <&cam_sensor_mclk0_active
			&cam_sensor_active_rst0>;
        //gpio 引脚第二个模式的配置
		pinctrl-1 = <&cam_sensor_mclk0_suspend
			&cam_sensor_suspend_rst0>;
        //gpio 具体使用的是哪个引脚
		gpios = <&tlmm 89 0>,
			<&tlmm 13 0>;
        //gpio 是否有复位功能
		gpio-reset = <1>;
        //gpio 请求的数量
		gpio-req-tbl-num = <0 1>;
        //gpio 有复位功能引脚的标志，有的置为一
		gpio-req-tbl-flags = <1 0>;
        //gpio 引脚分配标签
        gpio-req-tbl-label = "CAMIF_MCLK0",
					"CAM_RESET0";

        //sensor 使用的cci i2c控制系统是第几个
		cci-master = <CCI_MASTER_0>;
        //时钟的配置
		clocks = <&cambistmclkcc CAM_BIST_MCLK_CC_MCLK0_CLK>;
		clock-names = "cam_clk";
		clock-cntl-level = "nominal";
		clock-rates = <24000000>;
		status = "ok";
	};

