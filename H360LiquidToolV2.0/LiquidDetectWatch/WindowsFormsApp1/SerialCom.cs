using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;

namespace WindowsFormsApp1.BaseProtocol
{
    public class SerialCom
    {
        // 系统串口对象
        private SerialPort comm;
        //声明委托,用于在界面中实时显示串口传输讯息
        public delegate void AppendTranSerialMessage(byte dir, int length, ref byte[] buffer);
        //创建委托
        public AppendTranSerialMessage appendMessageTransSerialMessage;
        //一个实例只能创建一次委托

        //线程传输限制信号量
        private Mutex innerMutex = null;

        //接收数组
        public Byte[] receiveBuffer = null;
        //接收数据数量
        public int receiveDataCount = 0;

        //构造函数
        public SerialCom()
        {
            comm = null;
            appendMessageTransSerialMessage = null;
            receiveBuffer = new Byte[4096];
            receiveDataCount = 0;
            innerMutex = new Mutex();
        }

        //析构函数
        ~SerialCom()
        {
            if (comm != null)
            {
                //关闭串口
                comm.Close();
            }
            comm = null;
            appendMessageTransSerialMessage = null;
        }

        //检查串口是否打开
        public bool SerialIsOpen()
        {
            if (comm == null)
                return false;
            else
                return comm.IsOpen;
        }

        //获取系统的串口设备列表
        public string[] GetSystemPortArray()
        {
            string[] ports = SerialPort.GetPortNames();
            Array.Sort(ports);
            return ports;
        }

        //打开指定的串口
        public uint OpenSerial(String portName)
        {
            if (comm != null)
            {
                comm.Close();
            }
            comm = new SerialPort();
            string str = new string(portName.ToCharArray());
            comm.PortName = str;
            comm.BaudRate = 38400;
            comm.RtsEnable = false;//根据实际情况吧
            //添加事件注册,串口接收消息事件  
            comm.DataReceived += SerialReceive;
            comm.DataBits = 8;
            comm.Parity = Parity.None;
            comm.ReadBufferSize = 4096;
            comm.ReceivedBytesThreshold = 5;
            try
            {
                comm.Open();
            }
            catch (System.Exception ex)
            {
                comm.Close();
                comm = null;
                Debug.WriteLine(ex.Message);
                return 1;
            }
            return 0;
        }

        //关闭串口
        public void CloseSerial()
        {
            if (comm != null)
                comm.Close();

        }

        //主机上传AD值
        //基准值
        public const byte cmdAdcValueBase = 0x00;
        //目标原始值
        public const byte cmdAdcValueTargetBase = 0x01;
        //目标滤波值
        public const byte cmdAdcValueTargetFilter = 0X02;
        //液面探测状态值
        public const byte cmdLiquidDetectState = 0X03;
        //液面校准过程值
        public const byte cmdLiquidAdjustValue = 0X04;

        //读取指令
        public const byte cmdReadParamThresholdValue = 0x20;
        public const byte cmdReadParamThresholdCount = 0x21;
        public const byte cmdReadParamSlopeValue = 0x22;
        public const byte cmdReadParamSlopeCount = 0x23;
        public const byte cmdReadParamRes5272 = 0x24;
        public const byte cmdReadParamSignalMs = 0x25;

        //自校正
        public const byte cmdRes5272Adjust = 0x60;

        //写入参数
        public const byte cmdWriteParamRes5272 = 0x40;

        //暂停数据上传
        public const byte cmdSetAdcUpdatePluse = 0x61;
        //探液使能禁用
        public const byte cmdSetLiquidEnable = 0x62;

        //发送指令
        public void SendCommand(byte cmd,UInt16 param)
        {
            byte[] sendBuf = new byte[5];
            sendBuf[3] = 0x0d;
            sendBuf[4] = 0x0a;
            sendBuf[0] = cmd;
            sendBuf[1] = (byte)(param >> 8);
            sendBuf[2] = (byte)(param);
            if(comm.IsOpen)
            {
                comm.Write(sendBuf, 0, 5);
            }
        }

        //串口接收数据的回调函数
        private void SerialReceive(object sender, SerialDataReceivedEventArgs e)
        {
            if (comm == null)
                receiveDataCount = 0;
            else
            {
                //先记录下来，避免某种原因，人为的原因，操作几次之间时间长，缓存不一致
                int n = comm.BytesToRead;
                //读取缓冲数据
                comm.Read(receiveBuffer, receiveDataCount, n);
                //增加接收计数
                receiveDataCount = n;
                //委托被设置的情况下,去显示这些数据
                if (appendMessageTransSerialMessage != null)
                {
                    appendMessageTransSerialMessage((byte)1, receiveDataCount, ref receiveBuffer);
                }
                receiveDataCount = 0;
            }

        }
    }
}
