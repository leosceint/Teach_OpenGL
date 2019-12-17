using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace CS_CallDLL
{
    public partial class Form1 : Form
    {
        const string dllname = "C:\\Projects\\AeroGLWinAPI\\Debug\\GL_DLL.dll";
        [DllImport(dllname)]
        private static extern int LIBIUP_Main(int width, int height);

        private Timer timer;
        private const int HUD_FREQ = 40; //Hz
        private static Image glass = global::CS_CallDLL.Properties.Resources.glass;
        private static Image hud = global::CS_CallDLL.Properties.Resources.HUD_without_glass;
        private IntPtr Hdc;

        public Form1(int width, int x, int y)
        {

            InitializeComponent();
            this.Location= new Point(x, y);

            this.ClientSize = new Size(width, (int)(width * 1.0f / glass.Width * glass.Height));
            this.timer = new System.Windows.Forms.Timer();
            //this.timer.Interval = (int)((1 / (double)HUD_FREQ) * 1000);
            //this.timer.Tick += new EventHandler(this.Refresh);
            //LIBIUP_Main(200, 300);
            
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            e.Graphics.DrawImage(glass, 0, 0, ClientSize.Width, ClientSize.Height);
            e.Graphics.DrawImage(hud, 0, 0, ClientSize.Width, ClientSize.Height);
            LIBIUP_Main(700, 100);
        }

        //private void Refresh(object sender, EventArgs args) 
        //{
        //LIBIUP_Main(200, 300);
        //}
    }
}
