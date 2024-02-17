namespace WindowsFormsApp1
{
    partial class H360LiquidDetectWatch
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(H360LiquidDetectWatch));
            this.groupBoxSerialControl = new System.Windows.Forms.GroupBox();
            this.buttonLiquidTrig = new System.Windows.Forms.Button();
            this.buttonContinue = new System.Windows.Forms.Button();
            this.buttonPluse = new System.Windows.Forms.Button();
            this.buttonSaveADC = new System.Windows.Forms.Button();
            this.buttonClearTrans = new System.Windows.Forms.Button();
            this.buttonOpenCloseSerial = new System.Windows.Forms.Button();
            this.comboBoxSelectSerial = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.listBoxShowTrig = new System.Windows.Forms.ListBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.chartShow = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxAdjust = new System.Windows.Forms.TextBox();
            this.buttonAdjust = new System.Windows.Forms.Button();
            this.listBoxShowMsg = new System.Windows.Forms.ListBox();
            this.textBoxSignalMs = new System.Windows.Forms.TextBox();
            this.buttonReadSignalMs = new System.Windows.Forms.Button();
            this.buttonWriteRes = new System.Windows.Forms.Button();
            this.textBoxRes5272 = new System.Windows.Forms.TextBox();
            this.buttonReadRes = new System.Windows.Forms.Button();
            this.textBoxSlopeCount = new System.Windows.Forms.TextBox();
            this.buttonReadSlopeCount = new System.Windows.Forms.Button();
            this.textBoxSlopeValue = new System.Windows.Forms.TextBox();
            this.buttonReadSlopeValue = new System.Windows.Forms.Button();
            this.textBoxThresholdCount = new System.Windows.Forms.TextBox();
            this.buttonReadThresholdCount = new System.Windows.Forms.Button();
            this.textBoxThresholdValue = new System.Windows.Forms.TextBox();
            this.buttonReadThresholdValue = new System.Windows.Forms.Button();
            this.timerUpData = new System.Windows.Forms.Timer(this.components);
            this.buttonDisableLiquidDetect = new System.Windows.Forms.Button();
            this.buttonEnableLiquidDetect = new System.Windows.Forms.Button();
            this.groupBoxSerialControl.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chartShow)).BeginInit();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxSerialControl
            // 
            this.groupBoxSerialControl.Controls.Add(this.buttonDisableLiquidDetect);
            this.groupBoxSerialControl.Controls.Add(this.buttonLiquidTrig);
            this.groupBoxSerialControl.Controls.Add(this.buttonEnableLiquidDetect);
            this.groupBoxSerialControl.Controls.Add(this.buttonContinue);
            this.groupBoxSerialControl.Controls.Add(this.buttonPluse);
            this.groupBoxSerialControl.Controls.Add(this.buttonSaveADC);
            this.groupBoxSerialControl.Controls.Add(this.buttonClearTrans);
            this.groupBoxSerialControl.Controls.Add(this.buttonOpenCloseSerial);
            this.groupBoxSerialControl.Controls.Add(this.comboBoxSelectSerial);
            this.groupBoxSerialControl.Controls.Add(this.label1);
            this.groupBoxSerialControl.Location = new System.Drawing.Point(12, 12);
            this.groupBoxSerialControl.Name = "groupBoxSerialControl";
            this.groupBoxSerialControl.Size = new System.Drawing.Size(246, 247);
            this.groupBoxSerialControl.TabIndex = 1;
            this.groupBoxSerialControl.TabStop = false;
            this.groupBoxSerialControl.Text = "串口选择";
            // 
            // buttonLiquidTrig
            // 
            this.buttonLiquidTrig.BackColor = System.Drawing.Color.Green;
            this.buttonLiquidTrig.Location = new System.Drawing.Point(18, 203);
            this.buttonLiquidTrig.Name = "buttonLiquidTrig";
            this.buttonLiquidTrig.Size = new System.Drawing.Size(99, 37);
            this.buttonLiquidTrig.TabIndex = 19;
            this.buttonLiquidTrig.UseVisualStyleBackColor = false;
            // 
            // buttonContinue
            // 
            this.buttonContinue.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonContinue.Location = new System.Drawing.Point(123, 121);
            this.buttonContinue.Name = "buttonContinue";
            this.buttonContinue.Size = new System.Drawing.Size(101, 37);
            this.buttonContinue.TabIndex = 18;
            this.buttonContinue.Text = "开始ADC上传";
            this.buttonContinue.UseVisualStyleBackColor = true;
            this.buttonContinue.Click += new System.EventHandler(this.buttonContinue_Click);
            // 
            // buttonPluse
            // 
            this.buttonPluse.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonPluse.Location = new System.Drawing.Point(18, 121);
            this.buttonPluse.Name = "buttonPluse";
            this.buttonPluse.Size = new System.Drawing.Size(101, 37);
            this.buttonPluse.TabIndex = 17;
            this.buttonPluse.Text = "暂停ADC上传";
            this.buttonPluse.UseVisualStyleBackColor = true;
            this.buttonPluse.Click += new System.EventHandler(this.buttonPluse_Click);
            // 
            // buttonSaveADC
            // 
            this.buttonSaveADC.Location = new System.Drawing.Point(18, 164);
            this.buttonSaveADC.Name = "buttonSaveADC";
            this.buttonSaveADC.Size = new System.Drawing.Size(103, 37);
            this.buttonSaveADC.TabIndex = 16;
            this.buttonSaveADC.Text = "保存ADC数据";
            this.buttonSaveADC.UseVisualStyleBackColor = true;
            this.buttonSaveADC.Click += new System.EventHandler(this.buttonSaveADC_Click);
            // 
            // buttonClearTrans
            // 
            this.buttonClearTrans.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonClearTrans.Location = new System.Drawing.Point(16, 77);
            this.buttonClearTrans.Name = "buttonClearTrans";
            this.buttonClearTrans.Size = new System.Drawing.Size(103, 37);
            this.buttonClearTrans.TabIndex = 15;
            this.buttonClearTrans.Text = "清数据列表";
            this.buttonClearTrans.UseVisualStyleBackColor = true;
            this.buttonClearTrans.Click += new System.EventHandler(this.buttonClearTrans_Click);
            // 
            // buttonOpenCloseSerial
            // 
            this.buttonOpenCloseSerial.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonOpenCloseSerial.Location = new System.Drawing.Point(123, 77);
            this.buttonOpenCloseSerial.Name = "buttonOpenCloseSerial";
            this.buttonOpenCloseSerial.Size = new System.Drawing.Size(103, 37);
            this.buttonOpenCloseSerial.TabIndex = 14;
            this.buttonOpenCloseSerial.Text = "打开串口";
            this.buttonOpenCloseSerial.UseVisualStyleBackColor = true;
            this.buttonOpenCloseSerial.Click += new System.EventHandler(this.buttonOpenCloseSerial_Click);
            // 
            // comboBoxSelectSerial
            // 
            this.comboBoxSelectSerial.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxSelectSerial.FormattingEnabled = true;
            this.comboBoxSelectSerial.Location = new System.Drawing.Point(110, 47);
            this.comboBoxSelectSerial.Name = "comboBoxSelectSerial";
            this.comboBoxSelectSerial.Size = new System.Drawing.Size(114, 20);
            this.comboBoxSelectSerial.TabIndex = 13;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(119, 12);
            this.label1.TabIndex = 12;
            this.label1.Text = "串口选择(下拉更新):";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.listBoxShowTrig);
            this.groupBox2.Location = new System.Drawing.Point(13, 265);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(245, 764);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "接收信息显示";
            // 
            // listBoxShowTrig
            // 
            this.listBoxShowTrig.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listBoxShowTrig.FormattingEnabled = true;
            this.listBoxShowTrig.ItemHeight = 12;
            this.listBoxShowTrig.Location = new System.Drawing.Point(3, 17);
            this.listBoxShowTrig.Name = "listBoxShowTrig";
            this.listBoxShowTrig.Size = new System.Drawing.Size(239, 744);
            this.listBoxShowTrig.TabIndex = 0;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.tabControl1);
            this.groupBox3.Location = new System.Drawing.Point(265, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(1634, 1029);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "数据解析区间";
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(3, 17);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1628, 1009);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.chartShow);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1620, 983);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "AD值实时显示";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // chartShow
            // 
            chartArea1.Name = "ChartArea1";
            this.chartShow.ChartAreas.Add(chartArea1);
            this.chartShow.Dock = System.Windows.Forms.DockStyle.Fill;
            legend1.Name = "Legend1";
            this.chartShow.Legends.Add(legend1);
            this.chartShow.Location = new System.Drawing.Point(3, 3);
            this.chartShow.Name = "chartShow";
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.chartShow.Series.Add(series1);
            this.chartShow.Size = new System.Drawing.Size(1614, 977);
            this.chartShow.TabIndex = 0;
            this.chartShow.Text = "chartShow";
            this.chartShow.DoubleClick += new System.EventHandler(this.chartShow_DoubleClick);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.label8);
            this.tabPage2.Controls.Add(this.label7);
            this.tabPage2.Controls.Add(this.label6);
            this.tabPage2.Controls.Add(this.label5);
            this.tabPage2.Controls.Add(this.label4);
            this.tabPage2.Controls.Add(this.label3);
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Controls.Add(this.textBoxAdjust);
            this.tabPage2.Controls.Add(this.buttonAdjust);
            this.tabPage2.Controls.Add(this.listBoxShowMsg);
            this.tabPage2.Controls.Add(this.textBoxSignalMs);
            this.tabPage2.Controls.Add(this.buttonReadSignalMs);
            this.tabPage2.Controls.Add(this.buttonWriteRes);
            this.tabPage2.Controls.Add(this.textBoxRes5272);
            this.tabPage2.Controls.Add(this.buttonReadRes);
            this.tabPage2.Controls.Add(this.textBoxSlopeCount);
            this.tabPage2.Controls.Add(this.buttonReadSlopeCount);
            this.tabPage2.Controls.Add(this.textBoxSlopeValue);
            this.tabPage2.Controls.Add(this.buttonReadSlopeValue);
            this.tabPage2.Controls.Add(this.textBoxThresholdCount);
            this.tabPage2.Controls.Add(this.buttonReadThresholdCount);
            this.tabPage2.Controls.Add(this.textBoxThresholdValue);
            this.tabPage2.Controls.Add(this.buttonReadThresholdValue);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(1620, 983);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "数据控制";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(134, 379);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(233, 12);
            this.label8.TabIndex = 27;
            this.label8.Text = "RES5272电阻值自动调整,使基准电压为2.5V";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(134, 332);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(191, 12);
            this.label7.TabIndex = 26;
            this.label7.Text = "触发探液信号的锁定时间,单位毫秒";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(259, 270);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(167, 12);
            this.label6.TabIndex = 25;
            this.label6.Text = "当前写入RES5272的滑动电阻值";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(134, 208);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(113, 12);
            this.label5.TabIndex = 24;
            this.label5.Text = "探液板斜率检测次数";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(134, 140);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(239, 12);
            this.label4.TabIndex = 23;
            this.label4.Text = "探液板斜率检测,多次高于该斜率将触发探液";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(134, 82);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(113, 12);
            this.label3.TabIndex = 22;
            this.label3.Text = "探液板阈值检测次数";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(134, 16);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(239, 12);
            this.label2.TabIndex = 21;
            this.label2.Text = "探液板阈值检测,多次高于该阈值将触发探液";
            // 
            // textBoxAdjust
            // 
            this.textBoxAdjust.Location = new System.Drawing.Point(135, 400);
            this.textBoxAdjust.Name = "textBoxAdjust";
            this.textBoxAdjust.Size = new System.Drawing.Size(100, 21);
            this.textBoxAdjust.TabIndex = 20;
            // 
            // buttonAdjust
            // 
            this.buttonAdjust.Location = new System.Drawing.Point(7, 377);
            this.buttonAdjust.Name = "buttonAdjust";
            this.buttonAdjust.Size = new System.Drawing.Size(120, 53);
            this.buttonAdjust.TabIndex = 19;
            this.buttonAdjust.Text = "自校正";
            this.buttonAdjust.UseVisualStyleBackColor = true;
            this.buttonAdjust.Click += new System.EventHandler(this.buttonAdjust_Click);
            // 
            // listBoxShowMsg
            // 
            this.listBoxShowMsg.FormattingEnabled = true;
            this.listBoxShowMsg.ItemHeight = 12;
            this.listBoxShowMsg.Location = new System.Drawing.Point(7, 474);
            this.listBoxShowMsg.Name = "listBoxShowMsg";
            this.listBoxShowMsg.Size = new System.Drawing.Size(352, 496);
            this.listBoxShowMsg.TabIndex = 18;
            // 
            // textBoxSignalMs
            // 
            this.textBoxSignalMs.Location = new System.Drawing.Point(135, 347);
            this.textBoxSignalMs.Name = "textBoxSignalMs";
            this.textBoxSignalMs.Size = new System.Drawing.Size(100, 21);
            this.textBoxSignalMs.TabIndex = 16;
            // 
            // buttonReadSignalMs
            // 
            this.buttonReadSignalMs.Location = new System.Drawing.Point(7, 318);
            this.buttonReadSignalMs.Name = "buttonReadSignalMs";
            this.buttonReadSignalMs.Size = new System.Drawing.Size(120, 53);
            this.buttonReadSignalMs.TabIndex = 15;
            this.buttonReadSignalMs.Text = "读取信号保持时间";
            this.buttonReadSignalMs.UseVisualStyleBackColor = true;
            this.buttonReadSignalMs.Click += new System.EventHandler(this.buttonReadSignalMs_Click);
            // 
            // buttonWriteRes
            // 
            this.buttonWriteRes.Location = new System.Drawing.Point(133, 256);
            this.buttonWriteRes.Name = "buttonWriteRes";
            this.buttonWriteRes.Size = new System.Drawing.Size(120, 53);
            this.buttonWriteRes.TabIndex = 14;
            this.buttonWriteRes.Text = "写入滑动电阻值";
            this.buttonWriteRes.UseVisualStyleBackColor = true;
            this.buttonWriteRes.Click += new System.EventHandler(this.buttonWriteRes_Click);
            // 
            // textBoxRes5272
            // 
            this.textBoxRes5272.Location = new System.Drawing.Point(260, 285);
            this.textBoxRes5272.Name = "textBoxRes5272";
            this.textBoxRes5272.Size = new System.Drawing.Size(100, 21);
            this.textBoxRes5272.TabIndex = 13;
            // 
            // buttonReadRes
            // 
            this.buttonReadRes.Location = new System.Drawing.Point(7, 256);
            this.buttonReadRes.Name = "buttonReadRes";
            this.buttonReadRes.Size = new System.Drawing.Size(120, 53);
            this.buttonReadRes.TabIndex = 12;
            this.buttonReadRes.Text = "读取滑动电阻值";
            this.buttonReadRes.UseVisualStyleBackColor = true;
            this.buttonReadRes.Click += new System.EventHandler(this.buttonReadRes_Click);
            // 
            // textBoxSlopeCount
            // 
            this.textBoxSlopeCount.Location = new System.Drawing.Point(135, 223);
            this.textBoxSlopeCount.Name = "textBoxSlopeCount";
            this.textBoxSlopeCount.Size = new System.Drawing.Size(100, 21);
            this.textBoxSlopeCount.TabIndex = 10;
            // 
            // buttonReadSlopeCount
            // 
            this.buttonReadSlopeCount.Location = new System.Drawing.Point(7, 194);
            this.buttonReadSlopeCount.Name = "buttonReadSlopeCount";
            this.buttonReadSlopeCount.Size = new System.Drawing.Size(120, 53);
            this.buttonReadSlopeCount.TabIndex = 9;
            this.buttonReadSlopeCount.Text = "读取斜率检测次数";
            this.buttonReadSlopeCount.UseVisualStyleBackColor = true;
            this.buttonReadSlopeCount.Click += new System.EventHandler(this.buttonReadSlopeCount_Click);
            // 
            // textBoxSlopeValue
            // 
            this.textBoxSlopeValue.Location = new System.Drawing.Point(134, 160);
            this.textBoxSlopeValue.Name = "textBoxSlopeValue";
            this.textBoxSlopeValue.Size = new System.Drawing.Size(100, 21);
            this.textBoxSlopeValue.TabIndex = 7;
            // 
            // buttonReadSlopeValue
            // 
            this.buttonReadSlopeValue.Location = new System.Drawing.Point(7, 132);
            this.buttonReadSlopeValue.Name = "buttonReadSlopeValue";
            this.buttonReadSlopeValue.Size = new System.Drawing.Size(120, 53);
            this.buttonReadSlopeValue.TabIndex = 6;
            this.buttonReadSlopeValue.Text = "读取检测斜率值";
            this.buttonReadSlopeValue.UseVisualStyleBackColor = true;
            this.buttonReadSlopeValue.Click += new System.EventHandler(this.buttonReadSlopeValue_Click);
            // 
            // textBoxThresholdCount
            // 
            this.textBoxThresholdCount.Location = new System.Drawing.Point(135, 99);
            this.textBoxThresholdCount.Name = "textBoxThresholdCount";
            this.textBoxThresholdCount.Size = new System.Drawing.Size(100, 21);
            this.textBoxThresholdCount.TabIndex = 4;
            // 
            // buttonReadThresholdCount
            // 
            this.buttonReadThresholdCount.Location = new System.Drawing.Point(7, 70);
            this.buttonReadThresholdCount.Name = "buttonReadThresholdCount";
            this.buttonReadThresholdCount.Size = new System.Drawing.Size(120, 53);
            this.buttonReadThresholdCount.TabIndex = 3;
            this.buttonReadThresholdCount.Text = "读取阈值检测次数";
            this.buttonReadThresholdCount.UseVisualStyleBackColor = true;
            this.buttonReadThresholdCount.Click += new System.EventHandler(this.buttonReadThresholdCount_Click);
            // 
            // textBoxThresholdValue
            // 
            this.textBoxThresholdValue.Location = new System.Drawing.Point(135, 38);
            this.textBoxThresholdValue.Name = "textBoxThresholdValue";
            this.textBoxThresholdValue.Size = new System.Drawing.Size(100, 21);
            this.textBoxThresholdValue.TabIndex = 1;
            // 
            // buttonReadThresholdValue
            // 
            this.buttonReadThresholdValue.Location = new System.Drawing.Point(7, 8);
            this.buttonReadThresholdValue.Name = "buttonReadThresholdValue";
            this.buttonReadThresholdValue.Size = new System.Drawing.Size(120, 53);
            this.buttonReadThresholdValue.TabIndex = 0;
            this.buttonReadThresholdValue.Text = "读取检测阈值";
            this.buttonReadThresholdValue.UseVisualStyleBackColor = true;
            this.buttonReadThresholdValue.Click += new System.EventHandler(this.buttonReadThresholdValue_Click);
            // 
            // timerUpData
            // 
            this.timerUpData.Interval = 20;
            this.timerUpData.Tick += new System.EventHandler(this.timerUpData_Tick);
            // 
            // buttonDisableLiquidDetect
            // 
            this.buttonDisableLiquidDetect.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.buttonDisableLiquidDetect.Location = new System.Drawing.Point(123, 204);
            this.buttonDisableLiquidDetect.Name = "buttonDisableLiquidDetect";
            this.buttonDisableLiquidDetect.Size = new System.Drawing.Size(101, 37);
            this.buttonDisableLiquidDetect.TabIndex = 21;
            this.buttonDisableLiquidDetect.Text = "禁用探液信号";
            this.buttonDisableLiquidDetect.UseVisualStyleBackColor = true;
            this.buttonDisableLiquidDetect.Click += new System.EventHandler(this.ButtonDisableLiquidDetect_Click);
            // 
            // buttonEnableLiquidDetect
            // 
            this.buttonEnableLiquidDetect.Location = new System.Drawing.Point(123, 164);
            this.buttonEnableLiquidDetect.Name = "buttonEnableLiquidDetect";
            this.buttonEnableLiquidDetect.Size = new System.Drawing.Size(103, 37);
            this.buttonEnableLiquidDetect.TabIndex = 20;
            this.buttonEnableLiquidDetect.Text = "启用探液信号";
            this.buttonEnableLiquidDetect.UseVisualStyleBackColor = true;
            this.buttonEnableLiquidDetect.Click += new System.EventHandler(this.ButtonEnableLiquidDetect_Click);
            // 
            // H360LiquidDetectWatch
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1904, 1041);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBoxSerialControl);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "H360LiquidDetectWatch";
            this.Text = "液面探测版集成调试工具V2.0";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.H360LiquidDetectWatch_FormClosing);
            this.groupBoxSerialControl.ResumeLayout(false);
            this.groupBoxSerialControl.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.chartShow)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxSerialControl;
        private System.Windows.Forms.Button buttonClearTrans;
        private System.Windows.Forms.Button buttonOpenCloseSerial;
        private System.Windows.Forms.ComboBox comboBoxSelectSerial;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartShow;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button buttonSaveADC;
        private System.Windows.Forms.Timer timerUpData;
        private System.Windows.Forms.Button buttonPluse;
        private System.Windows.Forms.TextBox textBoxAdjust;
        private System.Windows.Forms.Button buttonAdjust;
        private System.Windows.Forms.ListBox listBoxShowMsg;
        private System.Windows.Forms.TextBox textBoxSignalMs;
        private System.Windows.Forms.Button buttonReadSignalMs;
        private System.Windows.Forms.Button buttonWriteRes;
        private System.Windows.Forms.TextBox textBoxRes5272;
        private System.Windows.Forms.Button buttonReadRes;
        private System.Windows.Forms.TextBox textBoxSlopeCount;
        private System.Windows.Forms.Button buttonReadSlopeCount;
        private System.Windows.Forms.TextBox textBoxSlopeValue;
        private System.Windows.Forms.Button buttonReadSlopeValue;
        private System.Windows.Forms.TextBox textBoxThresholdCount;
        private System.Windows.Forms.Button buttonReadThresholdCount;
        private System.Windows.Forms.TextBox textBoxThresholdValue;
        private System.Windows.Forms.Button buttonReadThresholdValue;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonContinue;
        private System.Windows.Forms.Button buttonLiquidTrig;
        private System.Windows.Forms.ListBox listBoxShowTrig;
        private System.Windows.Forms.Button buttonDisableLiquidDetect;
        private System.Windows.Forms.Button buttonEnableLiquidDetect;
    }
}

