using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SettingsDialog
{
    public partial class Dialog : Form
    {
        public Dialog()
        {
            InitializeComponent();
            diffcombo.SelectedIndex = 0;
            modecombo.SelectedIndex = 0;
        }

        private void Diffleft_Click(object sender, EventArgs e)
        {
            diffcombo.SelectedIndex--;
            if (diffcombo.SelectedIndex < 0)
                diffcombo.SelectedIndex = 0;
        }

        private void Diffright_Click(object sender, EventArgs e)
        {
            if (diffcombo.SelectedIndex == diffcombo.Items.Count - 1)
                return;
            diffcombo.SelectedIndex++;            
        }

        private void Modeleft_Click(object sender, EventArgs e)
        {
            modecombo.SelectedIndex--;
            if (modecombo.SelectedIndex < 0)
                modecombo.SelectedIndex = 0;
        }

        private void Moderight_Click(object sender, EventArgs e)
        {
            if (modecombo.SelectedIndex == diffcombo.Items.Count - 1)
                return;
            modecombo.SelectedIndex++;
        }

        private void OK_Click(object sender, EventArgs e)
        {
            Save();
            this.Close();
        }

        private void Save()
        {
            string path = Path.Combine(Environment.CurrentDirectory, "Content");
#if DEBUG
            FolderBrowserDialog d = new FolderBrowserDialog();
            d.Description = "Choose the Content Directory used by Flip Or Flop";
            if (d.ShowDialog() != DialogResult.OK)
                return;
            path = d.SelectedPath;
#endif
            path = Path.Combine(path, "settings.ini");
            using (StreamWriter write = new StreamWriter(path, false))
            {
                write.WriteLine("[gameplay]");
                write.WriteLine("; difficulty can be 1: easy, 2: medium, 3: hard (breakfast, lunch, dinner)");
                write.WriteLine($"difficulty={diffcombo.SelectedIndex + 1}");
                write.WriteLine("; 1: Classic (3 shifts), 2: Time Attack, 3: Freestyle");
                write.WriteLine($"mode={modecombo.SelectedIndex + 1}");
                write.WriteLine("; Setting currently does nothing");
                write.WriteLine($"usespongebob=1");
            }
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
