namespace SettingsDialog
{
    partial class Dialog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.Cancel = new System.Windows.Forms.Button();
            this.OK = new System.Windows.Forms.Button();
            this.diffleft = new System.Windows.Forms.Button();
            this.diffright = new System.Windows.Forms.Button();
            this.diffcombo = new System.Windows.Forms.ComboBox();
            this.modecombo = new System.Windows.Forms.ComboBox();
            this.moderight = new System.Windows.Forms.Button();
            this.modeleft = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox2.Image = global::SettingsDialog.Properties.Resources.spongebob;
            this.pictureBox2.Location = new System.Drawing.Point(0, 246);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(367, 269);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 1;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox1.Image = global::SettingsDialog.Properties.Resources.logo;
            this.pictureBox1.Location = new System.Drawing.Point(120, 12);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(434, 268);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 21.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(346, 287);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(128, 33);
            this.label1.TabIndex = 2;
            this.label1.Text = "Settings";
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.Color.Transparent;
            this.groupBox1.Controls.Add(this.diffcombo);
            this.groupBox1.Controls.Add(this.diffright);
            this.groupBox1.Controls.Add(this.diffleft);
            this.groupBox1.ForeColor = System.Drawing.Color.White;
            this.groupBox1.Location = new System.Drawing.Point(352, 324);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(301, 64);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Difficulty";
            // 
            // groupBox2
            // 
            this.groupBox2.BackColor = System.Drawing.Color.Transparent;
            this.groupBox2.Controls.Add(this.modecombo);
            this.groupBox2.Controls.Add(this.modeleft);
            this.groupBox2.Controls.Add(this.moderight);
            this.groupBox2.ForeColor = System.Drawing.Color.White;
            this.groupBox2.Location = new System.Drawing.Point(352, 394);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(301, 64);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Gamemode";
            // 
            // Cancel
            // 
            this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Cancel.Location = new System.Drawing.Point(547, 467);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(106, 35);
            this.Cancel.TabIndex = 5;
            this.Cancel.Text = "Cancel";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // OK
            // 
            this.OK.Location = new System.Drawing.Point(435, 467);
            this.OK.Name = "OK";
            this.OK.Size = new System.Drawing.Size(106, 35);
            this.OK.TabIndex = 0;
            this.OK.Text = "OK";
            this.OK.UseVisualStyleBackColor = true;
            this.OK.Click += new System.EventHandler(this.OK_Click);
            // 
            // diffleft
            // 
            this.diffleft.ForeColor = System.Drawing.Color.Black;
            this.diffleft.Location = new System.Drawing.Point(6, 19);
            this.diffleft.Name = "diffleft";
            this.diffleft.Size = new System.Drawing.Size(42, 30);
            this.diffleft.TabIndex = 6;
            this.diffleft.Text = "<";
            this.diffleft.UseVisualStyleBackColor = true;
            this.diffleft.Click += new System.EventHandler(this.Diffleft_Click);
            // 
            // diffright
            // 
            this.diffright.ForeColor = System.Drawing.Color.Black;
            this.diffright.Location = new System.Drawing.Point(253, 19);
            this.diffright.Name = "diffright";
            this.diffright.Size = new System.Drawing.Size(42, 30);
            this.diffright.TabIndex = 1;
            this.diffright.Text = ">";
            this.diffright.UseVisualStyleBackColor = true;
            this.diffright.Click += new System.EventHandler(this.Diffright_Click);
            // 
            // diffcombo
            // 
            this.diffcombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.diffcombo.FormattingEnabled = true;
            this.diffcombo.ItemHeight = 13;
            this.diffcombo.Items.AddRange(new object[] {
            "BREAKFAST (Easy)",
            "LUNCH (Medium)",
            "DINNER (Hard)"});
            this.diffcombo.Location = new System.Drawing.Point(54, 24);
            this.diffcombo.Name = "diffcombo";
            this.diffcombo.Size = new System.Drawing.Size(193, 21);
            this.diffcombo.TabIndex = 7;
            // 
            // modecombo
            // 
            this.modecombo.FormattingEnabled = true;
            this.modecombo.Items.AddRange(new object[] {
            "CLASSIC",
            "TIME ATTACK",
            "FREESTYLE"});
            this.modecombo.Location = new System.Drawing.Point(54, 24);
            this.modecombo.Name = "modecombo";
            this.modecombo.Size = new System.Drawing.Size(193, 21);
            this.modecombo.TabIndex = 10;
            // 
            // moderight
            // 
            this.moderight.ForeColor = System.Drawing.Color.Black;
            this.moderight.Location = new System.Drawing.Point(253, 19);
            this.moderight.Name = "moderight";
            this.moderight.Size = new System.Drawing.Size(42, 30);
            this.moderight.TabIndex = 9;
            this.moderight.Text = ">";
            this.moderight.UseVisualStyleBackColor = true;
            this.moderight.Click += new System.EventHandler(this.Moderight_Click);
            // 
            // modeleft
            // 
            this.modeleft.ForeColor = System.Drawing.Color.Black;
            this.modeleft.Location = new System.Drawing.Point(6, 19);
            this.modeleft.Name = "modeleft";
            this.modeleft.Size = new System.Drawing.Size(42, 30);
            this.modeleft.TabIndex = 8;
            this.modeleft.Text = "<";
            this.modeleft.UseVisualStyleBackColor = true;
            this.modeleft.Click += new System.EventHandler(this.Modeleft_Click);
            // 
            // Dialog
            // 
            this.AcceptButton = this.OK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::SettingsDialog.Properties.Resources.back;
            this.CancelButton = this.Cancel;
            this.ClientSize = new System.Drawing.Size(665, 514);
            this.Controls.Add(this.OK);
            this.Controls.Add(this.Cancel);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.pictureBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.HelpButton = true;
            this.Name = "Dialog";
            this.Text = "Bloaty\'s Flip Or Flop Settings";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.Button OK;
        private System.Windows.Forms.ComboBox diffcombo;
        private System.Windows.Forms.Button diffright;
        private System.Windows.Forms.Button diffleft;
        private System.Windows.Forms.ComboBox modecombo;
        private System.Windows.Forms.Button modeleft;
        private System.Windows.Forms.Button moderight;
    }
}

