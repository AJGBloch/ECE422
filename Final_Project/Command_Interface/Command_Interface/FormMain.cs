﻿/* 
 * File: FormMain.cs  
 * Author: Anthony Bloch
 * Comments: This is the main for Lab10 for ECE422
 *           This program runs alongside a program running on the PIC24FJ256GA2017  connected via uart to this gui.
 *           Protocols are sent from the GUI which are processed and responded to accordingly by the PIC24.
 *           Functionality includes:
 *                  constantly blink LED1 with period of 1 second
 *               prompted by protocol...
 *                  turn on/off LED2
 *                  turn on LED3 for a period of time after which it turns off
 *                      this LED can be disabled while it is on, and its status can also be read
 *                  enable LED4 such that is blinks on/off at a given frequency
 *                      this LED can be disabled such that it remains off
 *                  read a digital input button
 *                  read 3 analog channel inputs
 *                  reset capability through push button
 *          information is sent back to the GUI via uart as needed
 * 
 * Revision history: 
 */
using System;
using System.IO.Ports;
using System.Windows.Forms;

using CmdInterface.common;

namespace CmdInterface
{
    public partial class FormMain : Form, IView_FormMain
    {
        private FormPortSettings _formPortSettings;
        private PortSettingsEntity _defaultSettings, _currentSettings;
        private CmdComposer _composer;

        private int new_ch = 0;
        private char ch_rx;
        private char mode_rx;
        private string str_rx;
        private int count_rx;
        private char ch_tx;
        private int ready_to_send_protocol;

        public FormMain()
        {
            InitializeComponent();

            //Initialize UI visual elements
            connectToolStripMenuItem.Enabled = true;
            disconnectToolStripMenuItem.Enabled = false;
            //tbAscii.ReadOnly = true;
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            _defaultSettings = new PortSettingsEntity();  //Object instance containing default settings
            _currentSettings = new PortSettingsEntity();  //Object instance storing current settings from user

            InitializeComPort(_defaultSettings);
            InitializeComPort(_currentSettings);

            _composer = new CmdComposer(this);
        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Close();
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void InitializeComPort(PortSettingsEntity handle)
        {
            string[] port_list = System.IO.Ports.SerialPort.GetPortNames();

            if (port_list.Length > 0)
            {
                handle.PortName = port_list[0];  //Default com port is the first one on the list
                handle.BaudRate = 9600;
                handle.DataBits = 8;
                handle.StopBits = System.IO.Ports.StopBits.One;
                handle.Parity = System.IO.Ports.Parity.None;
            }
        }

        private void Connect(PortSettingsEntity handle)
        {
            if (handle != null)
            {
                //Configure serial port
                serialPort1.PortName = handle.PortName;
                serialPort1.BaudRate = (int)handle.BaudRate;
                serialPort1.DataBits = handle.DataBits;
                serialPort1.Parity = handle.Parity;
                serialPort1.Handshake = handle.Handshake;

                serialPort1.ReadTimeout = handle.ReadTimeout;
                serialPort1.WriteTimeout = handle.WriteTimeout;
                serialPort1.DiscardNull = handle.DiscardNull;
                serialPort1.RtsEnable = handle.RtsEnable;
                serialPort1.NewLine = handle.Newline;
                serialPort1.Encoding = handle.Encoding;

                try
                {
                    if (serialPort1.IsOpen == false)
                    {
                        serialPort1.Open();  //attempt to open the configured serial port

                        if (serialPort1.IsOpen)
                        {
                            //Com Port connected
                            connectToolStripMenuItem.Enabled = false;
                            disconnectToolStripMenuItem.Enabled = true;
                            tbAscii.ReadOnly = false;
                        }
                    }
                }
                catch (Exception)
                {
                    MessageBox.Show("Unable to open serial port");
                }
            }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //Note: SerialPort object operates on a seperate thread.
            //      Therefore, DataReceived event can not interact directly with other WinForm controls.
            //      Doing so will cause cross-thread action exception.
            //      In order to display received data to UI, delegate method must be used.

            //Read data from serial port object
            int temp;
            temp = serialPort1.ReadByte();
            ch_rx = (char)temp;
            if(ch_rx == '<') // begin protocol character
            {
                count_rx = 0;
                str_rx = "";
                temp = serialPort1.ReadByte();
                ch_rx = (char)temp;
                while (ch_rx != '>') // keep receiving protocol until end protocol character is received
                {
                    if(count_rx == 0)
                    {
                        mode_rx = ch_rx;
                    }
                    else
                    {
                        str_rx += ch_rx.ToString();
                    }
                    count_rx++;
                    temp = serialPort1.ReadByte();
                    ch_rx = (char)temp;
                }

            }
            else if(ch_rx == '!') // response from PIC24 that it is ready for protocol
            {
                ready_to_send_protocol = 1;
            }
            else
            {
                ;
            }
            this.Invoke(new EventHandler(ShowText));



        }

        private void ShowText(object sender, EventArgs e) // called after UART data received
        {
            if (count_rx > 0)
            {
                switch (mode_rx)
                {
                    case 'e': // status of timed output
                    {
                        if (count_rx > 1)
                        {
                            if (str_rx == "1")
                            {
                                tbStatus.Text = "ON";
                            }
                            else
                            {
                                tbStatus.Text = "OFF";
                            }
                        }
                        break;
                    }
                    case 'h': // digital read
                    {
                        if (count_rx > 1)
                        {
                            if (str_rx == "1")
                            {
                                tbInputStatus.Text = "ON";
                            }
                            else
                            {
                                tbInputStatus.Text = "OFF";
                            }
                        }
                        break;
                    }
                    case 'i': //read analog channel 1
                    {
                        if (count_rx > 1)
                        {
                            tbStatusAN1.Text = str_rx;
                        }
                        break;
                    }
                    case 'j': // read analog channel 2
                    {
                        if (count_rx > 1)
                        {
                            tbStatusAN2.Text = str_rx;
                        }
                        break;
                    }
                    case 'k': // read analog channel 3
                    {
                        if (count_rx > 1)
                        {
                            tbStatusAN3.Text = str_rx;
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }

        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //handle serial port connect
            Connect(_currentSettings);
        }

        private void disconnectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                //Disconnect serial port
                serialPort1.Close();

                connectToolStripMenuItem.Enabled = true;
                disconnectToolStripMenuItem.Enabled = false;
                tbAscii.ReadOnly = true;
            }
        }

        private void tbAscii_TextChanged(object sender, EventArgs e)
        {
            byte[] buffer = common.Utility.StringToByteArray(tbAscii.Text);
            UpdateHexTextbox((buffer));
        }

        public void ShowCommand(string command_string)
        {
            tbAscii.Text = command_string;
        }

        private void UpdateHexTextbox(byte[] buffer)
        {
            tbHex.Text = common.Utility.ByteArrayToString(buffer);
            tbHex.SelectionStart = tbHex.Text.Length;
            tbHex.ScrollToCaret();
            tbHex.Refresh();
        }

        private void clearTextToolStripMenuItem_Click(object sender, EventArgs e)
        {
            tbAscii.Clear();
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                if (string.IsNullOrEmpty(tbAscii.Text) == false)
                {
                    //Write string directly to serial port object
                    ready_to_send_protocol = 0;
                    serialPort1.Write("<"); // send begin protocol character
                    while (ready_to_send_protocol == 0); // wait for acknowledge from PIC24
                    serialPort1.Write(tbAscii.Text);
                    serialPort1.Write(">"); // send end protocol character
                }
            }
        }

        private void settingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_formPortSettings == null)
            {
                _formPortSettings = new FormPortSettings();
            }

            _formPortSettings.Initialize(_defaultSettings, _currentSettings);
            if (_formPortSettings.ShowDialog() == DialogResult.OK)
            {
                //apply settings
                _currentSettings = _formPortSettings.NewSettings;
            }
        }

        private void btnLedOff_Click(object sender, EventArgs e)
        {
            _composer.GenericCmd("b");
        }

        private void btnLedOn_Click(object sender, EventArgs e)
        {
            _composer.GenericCmd("a");
        }

        private void btnEnableTimed_Click(object sender, EventArgs e)
        {
            int parsedValue;
            if (int.TryParse(textBoxTime1.Text, out parsedValue))
            {
                _composer.GenericCmd("c" + textBoxTime1.Text);
            }
            else
            {
                _composer.GenericCmd("");
            }
            
        }

        private void btnDisableTimed_Click(object sender, EventArgs e)
        {
            _composer.GenericCmd("d");
        }

        private void btnEnablePeriodic_Click(object sender, EventArgs e)
        {
            int parsedValue;
            if (int.TryParse(tbPeriodic.Text, out parsedValue))
            {
                _composer.GenericCmd("f" + tbPeriodic.Text);
            }
            else
            {
                _composer.GenericCmd("");
            }
        }

        private void btnDisabledPeriodic_Click(object sender, EventArgs e)
        {
            _composer.GenericCmd("g");
        }

        private void btnReadInput_Click(object sender, EventArgs e)
        {
            _composer.GenericCmd("h");
        }

        private void btnCh1_Click(object sender, EventArgs e)
        {
            _composer.GenericCmd("i");
        }

        private void btnCh2_Click(object sender, EventArgs e)
        {
            _composer.GenericCmd("j");
        }

        private void tbStatus_TextChanged(object sender, EventArgs e)
        {
        }

        private void tbPeriodic_TextChanged(object sender, EventArgs e)
        {
        }

        private void lblSecond_Click(object sender, EventArgs e)
        {

        }

        private void btnStatus_Click(object sender, EventArgs e)
        {
            _composer.GenericCmd("e");
        }

        private void tbInputStatus_TextChanged(object sender, EventArgs e)
        {

        }

        private void tbStatusAN1_TextChanged(object sender, EventArgs e)
        {

        }

        private void btnCh3_Click(object sender, EventArgs e)
        {
            _composer.GenericCmd("k");
        }
    }
}
