
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;


namespace RegAnalyzer
{
    public partial class frmRegAnalyzer : Form
    {
    	// define the active/inactive color for all bits
    	private static Color activeColor   = Color.BurlyWood;
    	private static Color inactiveColor = System.Drawing.SystemColors.Window;
    	
        /// <summary>
        /// Constructor
        /// </summary>
        public frmRegAnalyzer()
        {
            InitializeComponent();
            
            // hook text click event handler
            Label[] txtBits = {txtBit0,  txtBit1,  txtBit2,  txtBit3,  txtBit4,  txtBit5,  txtBit6 , txtBit7,  txtBit8,  txtBit9, 
                               txtBit10, txtBit11, txtBit12, txtBit13, txtBit14, txtBit15, txtBit16, txtBit17, txtBit18, txtBit19,
                               txtBit20, txtBit21, txtBit22, txtBit23, txtBit24, txtBit25, txtBit26, txtBit27, txtBit28, txtBit29,
                               txtBit30, txtBit31};
			foreach (Label tb in txtBits)
			{
				tb.Click += new System.EventHandler(this.txtBit_Click);
			}
			
			// hook label click event handler
            Label[] lblTexts = {lblText0,  lblText1,  lblText2,  lblText3,  lblText4,  lblText5,  lblText6 , lblText7,  lblText8,  lblText9, 
                               lblText10, lblText11, lblText12, lblText13, lblText14, lblText15, lblText16, lblText17, lblText18, lblText19,
                               lblText20, lblText21, lblText22, lblText23, lblText24, lblText25, lblText26, lblText27, lblText28, lblText29,
                               lblText30, lblText31};
			foreach (Label tb in lblTexts)
			{
				tb.Click += new System.EventHandler(this.lblText_Click);
			}
        }

        /// <summary>
        /// Update Register Value
        /// </summary>
        private void UpdateRegisterValue()
        {
            Label[] txtBits = {txtBit0,  txtBit1,  txtBit2,  txtBit3,  txtBit4,  txtBit5,  txtBit6 , txtBit7,  txtBit8,  txtBit9, 
                               txtBit10, txtBit11, txtBit12, txtBit13, txtBit14, txtBit15, txtBit16, txtBit17, txtBit18, txtBit19,
                               txtBit20, txtBit21, txtBit22, txtBit23, txtBit24, txtBit25, txtBit26, txtBit27, txtBit28, txtBit29,
                               txtBit30, txtBit31};

            UInt32 value = 0;
            for (int i = 0; i < 32; i++)
            {
                if (txtBits[i].Text == "1")
                {
                    value += (UInt32)(1 << i);
                }
            }

            if (rbtHex.Checked == true)
            {
                txtRegValue.Text = value.ToString("X");
            }
            else
            {
                txtRegValue.Text = value.ToString();
            }
        }

        /// <summary>
        /// Label Text Click Handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void lblText_Click(object sender, EventArgs e)
        {
            Label[] lblTexts = {lblText0,  lblText1,  lblText2,  lblText3,  lblText4,  lblText5,  lblText6 , lblText7,  lblText8,  lblText9, 
                               lblText10, lblText11, lblText12, lblText13, lblText14, lblText15, lblText16, lblText17, lblText18, lblText19,
                               lblText20, lblText21, lblText22, lblText23, lblText24, lblText25, lblText26, lblText27, lblText28, lblText29,
                               lblText30, lblText31};
            
            if (lblText15.Text == "15")
            {
                // dec -> hex
    			foreach (Label tb in lblTexts)
    			{
    			    tb.Text = Convert.ToString(Convert.ToByte(tb.Text,10), 16).ToUpper();
    			}
            }
            else
            {
                // hex -> dec
    			foreach (Label tb in lblTexts)
    			{
    			    tb.Text = Convert.ToString(Convert.ToByte(tb.Text,16), 10);
    			}
            }
        }
        
        /// <summary>
        /// Bit Click Handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void txtBit_Click(object sender, EventArgs e)
        {
            Label txtClicked = (Label)sender;
            
            switch (txtClicked.Text)
            {
                case "0":
                    txtClicked.Text = "1";
                    txtClicked.BackColor = activeColor;
                    break;

                case "1":
                    txtClicked.Text = "0";
                    txtClicked.BackColor = inactiveColor;
                    break;

                default:
                    throw new ApplicationException("Unexpceted value!");
            }

            UpdateRegisterValue();
        }

        /// <summary>
        /// Hex checked changed handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void rbtHex_CheckedChanged(object sender, EventArgs e)
        {
            UpdateRegisterValue();
        }

        /// <summary>
        /// Check if RegValue is valid
        /// </summary>
        /// <returns></returns>
        private bool isRegValueValid()
        {
            // Null, return
            if (txtRegValue.Text.Length <= 0)
            {
                return false;
            }

            // Invalid character, return
            string tmpStr = txtRegValue.Text.Trim().ToUpper();
            foreach (char c in tmpStr)
            {
                switch (rbtHex.Checked)
                {
                    case true:	// Hex
                        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))
                        {
                            // do nothing
                        }
                        else
                        {
                            return false;
                        }
                        break;

                    case false:	// Dec
                        if (c >= '0' && c <= '9')
                        {
                            // do nothing
                        }
                        else
                        {
                            return false;
                        }
                        break;

                    default:
                        return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Check if ShiftValue is valid
        /// </summary>
        /// <returns></returns>
        private bool isShiftValueValid()
        {
            // Null, return
            if (txtShift.Text.Length <= 0)
            {
                return false;
            }

            // Invalid character, return
            txtShift.Text = txtShift.Text.Trim().ToUpper();
            foreach (char c in txtShift.Text)
            {
                if (c >= '0' && c <= '9')
                {
                    // do nothing
                }
                else
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Text Changed Handler
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void txtRegValue_TextChanged(object sender, EventArgs e)
        {
            Label[] txtBits = {txtBit0,  txtBit1,  txtBit2,  txtBit3,  txtBit4,  txtBit5,  txtBit6 , txtBit7,  txtBit8,  txtBit9, 
                               txtBit10, txtBit11, txtBit12, txtBit13, txtBit14, txtBit15, txtBit16, txtBit17, txtBit18, txtBit19,
                               txtBit20, txtBit21, txtBit22, txtBit23, txtBit24, txtBit25, txtBit26, txtBit27, txtBit28, txtBit29,
                               txtBit30, txtBit31};

        	UInt32 value;
        	
            if (isRegValueValid() == false)
            {
            	// invalid register value, force all text bits display 0
                value = 0;
            }
			else
			{
	            if (rbtHex.Checked == true)
	            {
	                value = Convert.ToUInt32(txtRegValue.Text, 16);
	            }
	            else
	            {
	                value = Convert.ToUInt32(txtRegValue.Text, 10);
	            }
			}

            for (int i = 0; i < 32; i++)
            {
                if (((value >> i) & 0x1) == 0x1)
                {
                    txtBits[i].Text = "1";
                    txtBits[i].BackColor = activeColor;
                }
                else
                {
                    txtBits[i].Text = "0";
                    txtBits[i].BackColor = inactiveColor;
                }
            }
        }

        /// <summary>
        /// Left shift
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnLeftShift_Click(object sender, EventArgs e)
        {
            if (isRegValueValid() == false)
            {
                return;
            }

            if (isShiftValueValid() == false)
            {
                return;
            }

            UInt32 value = 0;
            if (rbtHex.Checked == true)
            {
                value = Convert.ToUInt32(txtRegValue.Text, 16);
            }
            else
            {
                value = Convert.ToUInt32(txtRegValue.Text, 10);
            }

            value = value << Convert.ToByte(txtShift.Text);

            if (rbtHex.Checked == true)
            {
                txtRegValue.Text = value.ToString("X");
            }
            else
            {
                txtRegValue.Text = value.ToString();
            }
        }

        /// <summary>
        /// Right Shift
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnRightShift_Click(object sender, EventArgs e)
        {
            if (isRegValueValid() == false)
            {
                return;
            }

            if (isShiftValueValid() == false)
            {
                return;
            }

            UInt32 value = 0;
            if (rbtHex.Checked == true)
            {
                value = Convert.ToUInt32(txtRegValue.Text, 16);
            }
            else
            {
                value = Convert.ToUInt32(txtRegValue.Text, 10);
            }

            value = value >> Convert.ToByte(txtShift.Text);

            if (rbtHex.Checked == true)
            {
                txtRegValue.Text = value.ToString("X");
            }
            else
            {
                txtRegValue.Text = value.ToString();
            }
        }

        /// <summary>
        /// Clear register value
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnClear_Click(object sender, EventArgs e)
        {
            txtRegValue.Text = "0";

            txtRegValue_TextChanged(null, null);
        }

        /// <summary>
        /// Reverse
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnReverse_Click(object sender, EventArgs e)
        {
        	Label[] txtBits = {txtBit0,  txtBit1,  txtBit2,  txtBit3,  txtBit4,  txtBit5,  txtBit6 , txtBit7,  txtBit8,  txtBit9,
        		               txtBit10, txtBit11, txtBit12, txtBit13, txtBit14, txtBit15, txtBit16, txtBit17, txtBit18, txtBit19,
        		               txtBit20, txtBit21, txtBit22, txtBit23, txtBit24, txtBit25, txtBit26, txtBit27, txtBit28, txtBit29,
        		               txtBit30, txtBit31};

        	foreach (Label tb in txtBits)
        	{
        		switch (tb.Text)
        		{
        			case "0":
        				tb.Text = "1";
        				tb.BackColor = activeColor;
        				break;

        			case "1":
        				tb.Text = "0";
        				tb.BackColor = inactiveColor;
        				break;

        			default:
        				throw new ApplicationException("Unexpceted value!");
        		}
        	}

        	UpdateRegisterValue();
        }
        
        /// <summary>
        /// Set On-Top or not
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void ChkOnTopCheckedChanged(object sender, EventArgs e)
        {
        	this.TopMost = chkOnTop.Checked;
        }

        /// <summary>
        /// Close Form if ESC is pressed
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void frmRegAnalyzer_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Escape:
                    this.Close();
                    break;

                default:
                    break;
            }
        }
        
        /// <summary>
        /// Close
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void BtnCloseClick(object sender, EventArgs e)
        {
        	this.Close();
        }
    }
}
