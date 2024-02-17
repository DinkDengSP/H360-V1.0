using System;
using System.Collections;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using WindowsFormsApp1.BaseProtocol;

namespace WindowsFormsApp1
{
    public partial class H360LiquidDetectWatch : Form
    {
        //基础ADC
        Series seriesBase;
        //目标ADC
        Series seriesTarget;
        //串口消息
        SerialCom serialCom = null;
        //保存数据
        ArrayList baseAdcValue;
        ArrayList targetAdcValueSrc;
        ArrayList targetAdcValueFilter;
        ArrayList serialBufferList;
        public H360LiquidDetectWatch()
        {
            InitializeComponent();
            InitChart();
            //关闭跨线程调用检查
            CheckForIllegalCrossThreadCalls = false;
            serialCom = new SerialCom();
            serialCom.appendMessageTransSerialMessage = SerialRecvMsgProc;
            comboBoxSelectSerial.Items.Clear();
            comboBoxSelectSerial.Items.AddRange(serialCom.GetSystemPortArray());
            if (comboBoxSelectSerial.Items.Count > 0)
            {
                comboBoxSelectSerial.SelectedItem = 0;
            }
            baseAdcValue = new ArrayList();
            targetAdcValueSrc = new ArrayList();
            targetAdcValueFilter = new ArrayList();
            serialBufferList = new ArrayList();
            timerUpData.Start();
        }
        Mutex srMutex = new Mutex();

        //串口接收到的数据处理
        void SerialRecvMsgProc(byte dir, int length, ref byte[] buffer)
        {
            srMutex.WaitOne();
            //数据存下来
            for (int i = 0; i < length; i++)
            {
                serialBufferList.Add(buffer[i]);
            }
            srMutex.ReleaseMutex();
        }

        public void InitChart()
        {
            //曲线的初始化
            chartShow.ChartAreas.Clear();
            chartShow.Series.Clear();

            #region 设置图表的属性
            //图表的背景色
            chartShow.BackColor = Color.FromArgb(211, 223, 240);
            //图表背景色的渐变方式
            chartShow.BackGradientStyle = GradientStyle.TopBottom;
            //图表的边框颜色、
            chartShow.BorderlineColor = Color.FromArgb(26, 59, 105);
            //图表的边框线条样式
            chartShow.BorderlineDashStyle = ChartDashStyle.Solid;
            //图表边框线条的宽度
            chartShow.BorderlineWidth = 2;
            //图表边框的皮肤
            chartShow.BorderSkin.SkinStyle = BorderSkinStyle.Emboss;
            #endregion

            #region 设置图表的Title
            Title title = new Title();
            //标题内容
            title.Text = "探液板ADC检测曲线";
            //标题的字体
            title.Font = new System.Drawing.Font("Microsoft Sans Serif", 12, FontStyle.Bold);
            //标题字体颜色
            title.ForeColor = Color.FromArgb(26, 59, 105);
            //标题阴影颜色
            title.ShadowColor = Color.FromArgb(32, 0, 0, 0);
            //标题阴影偏移量
            title.ShadowOffset = 3;

            chartShow.Titles.Add(title);
            #endregion

            #region 设置图表区属性
            //图表区的名字
            ChartArea chartArea = new ChartArea("Default");
            //背景色
            chartArea.BackColor = Color.FromArgb(64, 165, 191, 228);
            //背景渐变方式
            chartArea.BackGradientStyle = GradientStyle.TopBottom;
            //渐变和阴影的辅助背景色
            chartArea.BackSecondaryColor = Color.White;
            //边框颜色
            chartArea.BorderColor = Color.FromArgb(64, 64, 64, 64);
            //阴影颜色
            chartArea.ShadowColor = Color.Transparent;

            //设置X轴和Y轴线条的颜色和宽度
            chartArea.AxisX.LineColor = Color.FromArgb(64, 64, 64, 64);
            chartArea.AxisX.LineWidth = 1;
            chartArea.AxisY.LineColor = Color.FromArgb(64, 64, 64, 64);
            chartArea.AxisY.LineWidth = 1;

            //设置X轴和Y轴的标题
            chartArea.AxisX.Title = "采样序号";
            chartArea.AxisY.Title = "ADC值";

            //设置图表区网格横纵线条的颜色和宽度
            chartArea.AxisX.MajorGrid.LineColor = Color.FromArgb(64, 64, 64, 64);
            chartArea.AxisX.MajorGrid.LineWidth = 1;
            chartArea.AxisY.MajorGrid.LineColor = Color.FromArgb(64, 64, 64, 64);
            chartArea.AxisY.MajorGrid.LineWidth = 1;

            //设置X轴允许拖动放大
            chartArea.CursorX.IsUserEnabled = true;
            chartArea.CursorX.IsUserSelectionEnabled = true;
            chartArea.CursorX.Interval = 1;
            chartArea.CursorX.IntervalOffset = 1;
            chartArea.CursorX.IntervalType = DateTimeIntervalType.Auto;
            chartArea.AxisX.ScaleView.Zoomable = true;
            chartArea.AxisX.ScrollBar.IsPositionedInside = false;
            chartArea.AxisX.IntervalOffset = 1.00D;
            //缩放级别
            chartArea.AxisX.ScaleView.SmallScrollMinSize = 1;
            chartArea.AxisX.ScaleView.SmallScrollSize = 1;

            //设定Y轴上下限
            chartArea.AxisY.Minimum = 0;
            chartArea.AxisY.Maximum = 5000;
            chartArea.AxisY.Interval = 100;

            chartShow.ChartAreas.Add(chartArea);
            #endregion

            #region 图例及图例的位置
            Legend legend = new Legend();
            legend.Alignment = StringAlignment.Center;
            legend.Docking = Docking.Bottom;

            this.chartShow.Legends.Add(legend);
            #endregion

            seriesBase = new Series("基准ADC曲线");

            //Series的类型
            seriesBase.ChartType = SeriesChartType.FastLine;
            //Series的边框颜色
            seriesBase.BorderColor = Color.FromArgb(180, 26, 59, 105);
            //线条宽度
            seriesBase.BorderWidth = 2;
            //线条阴影颜色
            seriesBase.ShadowColor = Color.Black;
            //阴影宽度
            seriesBase.ShadowOffset = 0;
            //是否显示数据说明
            seriesBase.IsVisibleInLegend = true;
            //线条上数据点上是否有数据显示
            seriesBase.IsValueShownAsLabel = false;
            //线条上的数据点标志类型
            seriesBase.MarkerStyle = MarkerStyle.Circle;
            //线条数据点的大小
            seriesBase.MarkerSize = 2;
            seriesBase.Color = Color.Blue;
            seriesBase.YAxisType = AxisType.Primary;

            this.chartShow.Series.Add(seriesBase);

            seriesTarget = new Series("目标原始ADC曲线");

            //Series的类型
            seriesTarget.ChartType = SeriesChartType.FastLine;
            //Series的边框颜色
            seriesTarget.BorderColor = Color.FromArgb(180, 26, 59, 105);
            //线条宽度
            seriesTarget.BorderWidth = 2;
            //线条阴影颜色
            seriesTarget.ShadowColor = Color.Black;
            //阴影宽度
            seriesTarget.ShadowOffset = 0;
            //是否显示数据说明
            seriesTarget.IsVisibleInLegend = true;
            //线条上数据点上是否有数据显示
            seriesTarget.IsValueShownAsLabel = false;
            //线条上的数据点标志类型
            seriesTarget.MarkerStyle = MarkerStyle.Circle;
            //线条数据点的大小
            seriesTarget.MarkerSize = 2;
            seriesTarget.Color = Color.DarkGreen;
            seriesTarget.YAxisType = AxisType.Primary;

            this.chartShow.Series.Add(seriesTarget);


            seriesTarget = new Series("目标滤波后ADC曲线");

            //Series的类型
            seriesTarget.ChartType = SeriesChartType.FastLine;
            //Series的边框颜色
            seriesTarget.BorderColor = Color.FromArgb(180, 26, 59, 105);
            //线条宽度
            seriesTarget.BorderWidth = 2;
            //线条阴影颜色
            seriesTarget.ShadowColor = Color.Black;
            //阴影宽度
            seriesTarget.ShadowOffset = 0;
            //是否显示数据说明
            seriesTarget.IsVisibleInLegend = true;
            //线条上数据点上是否有数据显示
            seriesTarget.IsValueShownAsLabel = false;
            //线条上的数据点标志类型
            seriesTarget.MarkerStyle = MarkerStyle.Circle;
            //线条数据点的大小
            seriesTarget.MarkerSize = 2;
            seriesTarget.Color = Color.DarkRed;
            seriesTarget.YAxisType = AxisType.Primary;

            this.chartShow.Series.Add(seriesTarget);

            //关联
            this.chartShow.GetToolTipText += new EventHandler<ToolTipEventArgs>(myChart_GetToolTipText);
        }

        private void myChart_GetToolTipText(object sender, ToolTipEventArgs e)
        {
            if (e.HitTestResult.ChartElementType == ChartElementType.DataPoint)
            {
                int i = e.HitTestResult.PointIndex;
                DataPoint dp = e.HitTestResult.Series.Points[i];
                e.Text = string.Format("序号:{0};数值:{1:F2} ", dp.XValue, dp.YValues[0]);
            }
        }
        //查看点值
        private void chartShow_DoubleClick(object sender, EventArgs e)
        {
            while (this.chartShow.ChartAreas[0].AxisX.ScaleView.IsZoomed)
            {
                this.chartShow.ChartAreas[0].AxisX.ScaleView.ZoomReset();
            }
        }
        //保存数据
        private void buttonSaveADC_Click(object sender, EventArgs e)
        {
            if (chartShow.Series[0].Points.Count > 0 && chartShow.Series[1].Points.Count > 0 && chartShow.Series[2].Points.Count > 0)
            {
                buttonSaveADC.Enabled = false;
                string saveFileName = "";
                SaveFileDialog saveDialog = new SaveFileDialog();
                saveFileName = saveDialog.FileName;

                saveDialog.Filter = "csv (*.csv)|*.csv";
                saveDialog.FilterIndex = 2;
                saveDialog.RestoreDirectory = true;

                if (saveDialog.ShowDialog() == DialogResult.OK)
                {
                    StringBuilder sb = new StringBuilder();
                    sb.AppendLine("序号,基准ADC,原始目标ADC,滤波目标ADC");
                    for (int i = 0; i < chartShow.Series[0].Points.Count; i++)
                    {
                        sb.AppendFormat("{0:D},{1:F2},{2:F2},{3:F2}\r\n", i, chartShow.Series[0].Points[i].YValues[0],
                            chartShow.Series[1].Points[i].YValues[0], chartShow.Series[2].Points[i].YValues[0]);
                    }

                    using (StreamWriter tempDataParam = new StreamWriter(saveDialog.FileName, false, Encoding.UTF8))
                    {
                        tempDataParam.Write(sb.ToString());
                        MessageBox.Show("写入完成");
                    }
                }
                buttonSaveADC.Enabled = true;
            }
            else
            {
                return;
            }
        }
        //增加基准ADC显示
        private void AppendBaseADCValue(int adcValue)
        {
            if (this.chartShow.Series[0].Points.Count >= 8002)
            {
                this.chartShow.Series[0].Points.RemoveAt(0);
            }
            //计算转换
            this.chartShow.Series[0].Points.Add(adcValue);
        }

        //增加目标ADC显示 原始值
        private void AppendTargetADCSrcValue(int adcValue)
        {
            if (this.chartShow.Series[1].Points.Count >= 8002)
            {
                this.chartShow.Series[1].Points.RemoveAt(0);
            }
            //计算转换
            this.chartShow.Series[1].Points.AddY(adcValue);
        }

        //增加目标ADC显示 滤波后
        private void AppendTargetADCFilterValue(int adcValue)
        {
            if (this.chartShow.Series[2].Points.Count >= 8002)
            {
                this.chartShow.Series[2].Points.RemoveAt(0);
            }
            //计算转换
            this.chartShow.Series[2].Points.AddY(adcValue);
        }

        //清除当前数据
        private void ClearCurrentData()
        {
            this.chartShow.Series[0].Points.Clear();
            this.chartShow.Series[1].Points.Clear();
            this.chartShow.Series[2].Points.Clear();
            while (chartShow.ChartAreas[0].AxisX.ScaleView.IsZoomed)
            {
                chartShow.ChartAreas[0].AxisX.ScaleView.ZoomReset();
            }
        }

        private void buttonOpenCloseSerial_Click(object sender, EventArgs e)
        {
            if (serialCom.SerialIsOpen() == false)
            {
                /**打开串口*/
                if (comboBoxSelectSerial.SelectedIndex >= 0)
                {
                    if (0 == serialCom.OpenSerial(comboBoxSelectSerial.SelectedItem.ToString()))
                    {
                        /**打开成功*/
                        buttonOpenCloseSerial.Text = "关闭串口";
                        comboBoxSelectSerial.Enabled = false;
                    }
                    else
                    {
                        MessageBox.Show("串口打开失败,请检查是否串口占用或损坏");
                    }
                }
                else
                {
                    MessageBox.Show("请选择一个有效串口");
                }
            }
            else
            {
                /**关闭串口*/
                serialCom.CloseSerial();
                comboBoxSelectSerial.Enabled = true;
                buttonOpenCloseSerial.Text = "打开串口";
            }
        }

        private void buttonClearTrans_Click(object sender, EventArgs e)
        {
            ClearCurrentData();
            listBoxShowTrig.Items.Clear();
        }

        private void H360LiquidDetectWatch_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialCom != null)
            {
                if (serialCom.SerialIsOpen())
                {
                    serialCom.CloseSerial();
                }
            }
            System.Environment.Exit(0);
        }

        //定时更新界面
        private void timerUpData_Tick(object sender, EventArgs e)
        {
            if (serialCom != null)
            {
                if (serialCom.SerialIsOpen())
                {
                    srMutex.WaitOne();
                    //数据解析
                    int currentDataLength = serialBufferList.Count;
                    int removeIndexMax = 0;
                    for (int i = 0; i < currentDataLength - 1; i++)
                    {
                        //找到结束符
                        if (((byte)serialBufferList[i] == 0x0d) && ((byte)serialBufferList[i + 1] == 0x0a))
                        {
                            int tempValue = 0;
                            //确定数据字节大于5
                            if (i >= 3)
                            {
                                if ((byte)serialBufferList[i - 3] == SerialCom.cmdAdcValueBase)
                                {
                                    //原始ADC
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    baseAdcValue.Add(tempValue);
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdAdcValueTargetBase)
                                {
                                    //目标ADC原始值
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    targetAdcValueSrc.Add(tempValue);
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdAdcValueTargetFilter)
                                {
                                    //目标ADC滤波值
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    targetAdcValueFilter.Add(tempValue);
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdLiquidAdjustValue)
                                {
                                    //校准过程值
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    baseAdcValue.Add(tempValue);
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdReadParamThresholdValue)
                                {
                                    //阈值检测值
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    //显示界面
                                    textBoxThresholdValue.Text = tempValue.ToString();
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdReadParamThresholdCount)
                                {
                                    //阈值检测次数
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    //显示界面
                                    textBoxThresholdCount.Text = tempValue.ToString();
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdReadParamSlopeValue)
                                {
                                    //斜率检测值
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    //显示界面
                                    textBoxSlopeValue.Text = tempValue.ToString();
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdReadParamSlopeCount)
                                {
                                    //斜率检测次数
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    //显示界面
                                    textBoxSlopeCount.Text = tempValue.ToString();
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdReadParamRes5272)
                                {
                                    //电阻值
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    //显示界面
                                    textBoxRes5272.Text = tempValue.ToString();
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdReadParamSignalMs)
                                {
                                    //信号保持时间
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    //显示界面
                                    textBoxSignalMs.Text = tempValue.ToString();
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdRes5272Adjust)
                                {
                                    //自校准结果
                                    tempValue = 0;
                                    tempValue = (byte)serialBufferList[i - 2];
                                    if (tempValue == 0)
                                    {
                                        //校准成功
                                        listBoxShowTrig.Items.Insert(0, "自校准成功");
                                        tempValue += (byte)serialBufferList[i - 1];
                                        tempValue *= 10;
                                        //显示界面校准结果
                                        textBoxAdjust.Text = tempValue.ToString();
                                    }
                                    else
                                    {
                                        textBoxAdjust.Text = "";
                                        if (tempValue == 1)
                                        {
                                            //校准失败
                                            listBoxShowTrig.Items.Insert(0, "自校准失败,电阻设置为最大基准电压依然不到4V");
                                        }
                                        else if(tempValue == 2)
                                        {
                                            //校准失败
                                            listBoxShowTrig.Items.Insert(0, "自校准失败,电阻设置为最小基准电压依然大于1V");
                                        }
                                        else if (tempValue == 3)
                                        {
                                            //校准失败
                                            listBoxShowTrig.Items.Insert(0, "自校准失败,无法找到让电压在2.4-2.6之间的参数");
                                        }
                                        else
                                        {
                                            //校准失败
                                            listBoxShowTrig.Items.Insert(0, "自校准失败,错误原因未知");
                                        }
                                    }
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdWriteParamRes5272)
                                {
                                    listBoxShowMsg.Items.Insert(0, "滑动电阻值写入成功");
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdSetAdcUpdatePluse)
                                {
                                    //信号保持时间
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    if(tempValue == 0)
                                    {
                                        //关闭上传成功
                                        listBoxShowTrig.Items.Insert(0, "关闭上传成功");
                                    }
                                    else
                                    {
                                        //打开上传成功
                                        listBoxShowTrig.Items.Insert(0, "打开上传成功");
                                    }
                                }
                                else if((byte)serialBufferList[i - 3] == SerialCom.cmdSetLiquidEnable)
                                {
                                    //信号保持时间
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    if (tempValue == 0)
                                    {
                                        //探液功能禁用成功
                                        listBoxShowTrig.Items.Insert(0, "探液功能禁用成功");
                                    }
                                    else
                                    {
                                        //探液功能启用成功
                                        listBoxShowTrig.Items.Insert(0, "探液功能启用成功");
                                    }
                                }
                                else if ((byte)serialBufferList[i - 3] == SerialCom.cmdLiquidDetectState)
                                {
                                    //探液信号触发
                                    tempValue = 0;
                                    tempValue += (byte)serialBufferList[i - 2];
                                    tempValue *= 256;
                                    tempValue += (byte)serialBufferList[i - 1];
                                    if (tempValue == 0)
                                    {
                                        //取消触发
                                        buttonLiquidTrig.BackColor = Color.Green;
                                    }
                                    else
                                    {
                                        //触发
                                        buttonLiquidTrig.BackColor = Color.Red;
                                        listBoxShowTrig.Items.Insert(0, "Trig : " + DateTime.Now.ToString());
                                    }
                                }
                                removeIndexMax = i + 1;
                            }
                        }
                    }
                    serialBufferList.RemoveRange(0, removeIndexMax);
                    //界面显示
                    if (baseAdcValue.Count > 0)
                    {
                        for (int i = 0; i < baseAdcValue.Count; i++)
                        {
                            AppendBaseADCValue((int)baseAdcValue[i]);
                        }
                    }
                    baseAdcValue.Clear();

                    if (targetAdcValueSrc.Count > 0)
                    {
                        for (int i = 0; i < targetAdcValueSrc.Count; i++)
                        {
                            AppendTargetADCSrcValue((int)targetAdcValueSrc[i]);
                        }
                    }
                    targetAdcValueSrc.Clear();

                    if (targetAdcValueFilter.Count > 0)
                    {
                        for (int i = 0; i < targetAdcValueFilter.Count; i++)
                        {
                            AppendTargetADCFilterValue((int)targetAdcValueFilter[i]);
                        }
                    }
                    targetAdcValueFilter.Clear();

                    srMutex.ReleaseMutex();
                }
            }
        }


        private void buttonPluse_Click(object sender, EventArgs e)
        {
            serialCom.SendCommand(SerialCom.cmdSetAdcUpdatePluse, 0);
        }
        private void buttonContinue_Click(object sender, EventArgs e)
        {
            serialCom.SendCommand(SerialCom.cmdSetAdcUpdatePluse, 1);
        }

        private void buttonReadThresholdValue_Click(object sender, EventArgs e)
        {
            //读取检测阈值
            serialCom.SendCommand(SerialCom.cmdReadParamThresholdValue, 0);
        }

        private void buttonReadThresholdCount_Click(object sender, EventArgs e)
        {
            //读取阈值检测次数
            serialCom.SendCommand(SerialCom.cmdReadParamThresholdCount, 0);
        }

        private void buttonReadSlopeValue_Click(object sender, EventArgs e)
        {
            //读取斜率检测值
            serialCom.SendCommand(SerialCom.cmdReadParamSlopeValue, 0);
        }

        private void buttonReadSlopeCount_Click(object sender, EventArgs e)
        {
            //读取斜率检测次数
            serialCom.SendCommand(SerialCom.cmdReadParamSlopeCount, 0);
        }

        private void buttonReadRes_Click(object sender, EventArgs e)
        {
            //读取电阻设置值
            serialCom.SendCommand(SerialCom.cmdReadParamRes5272, 0);
        }

        private void buttonReadSignalMs_Click(object sender, EventArgs e)
        {
            //读取信号保持时间
            serialCom.SendCommand(SerialCom.cmdReadParamSignalMs, 0);
        }

        private void buttonAdjust_Click(object sender, EventArgs e)
        {
            //提示用户
            listBoxShowTrig.Items.Insert(0, "自校准先关闭数据上传");
            //清除显示数据列表
            ClearCurrentData();
            //自我校准
            serialCom.SendCommand(SerialCom.cmdRes5272Adjust, 0);
        }

        private void buttonWriteRes_Click(object sender, EventArgs e)
        {
            UInt16 paramValue;
            //参数转换
            if (false == UInt16.TryParse(textBoxRes5272.Text, out paramValue))
            {
                listBoxShowMsg.Items.Insert(0, "参数转换失败");
            }
            serialCom.SendCommand(SerialCom.cmdWriteParamRes5272, paramValue);
            listBoxShowMsg.Items.Insert(0, "滑动电阻值写入已下发");
        }

        private void ButtonEnableLiquidDetect_Click(object sender, EventArgs e)
        {
            //使能探液功能
            serialCom.SendCommand(SerialCom.cmdSetLiquidEnable, 1);
        }

        private void ButtonDisableLiquidDetect_Click(object sender, EventArgs e)
        {
            //禁用探液功能
            serialCom.SendCommand(SerialCom.cmdSetLiquidEnable, 0);
        }
    }
}
