import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;

class NoteBookMain {
    private JFrame frame = new JFrame();
    private JMenuBar menuBar = new JMenuBar();
    private JTextArea textArea = new JTextArea();
    private JScrollPane scrollPane = new JScrollPane(textArea);

    private JMenu menu_File = new JMenu("File(F)");       //Alt + F
    private JMenu menu_Edit = new JMenu("Edit(E)");       //Alt + E
    private JMenu menu_Format = new JMenu("Format(O)");     //Alt + O
    private JMenu menu_Help = new JMenu("Help(H)");       //Alt + H

    private JMenuItem mi_File_New = new JMenuItem("New(N)");     //Ctrl + N
    private JMenuItem mi_File_Open = new JMenuItem("Open(O)");    //Ctrl + O
    private JMenuItem mi_File_Save = new JMenuItem("Save(S)");    //Ctrl + S
    private JMenuItem mi_File_Save_As = new JMenuItem("SaveAs(A)");    //Alt + A
    private JMenuItem mi_File_Exit = new JMenuItem("Exit(X)");        //Alt + X

    //private JMenuItem mi_Edict_Undo = new JMenuItem("Undo(Z)");   //Ctrl + Z
    private JMenuItem mi_Edict_Copy = new JMenuItem("Copy(C)");   //Ctrl + C
    private JMenuItem mi_Edict_Paste = new JMenuItem("Paste(V)");  //Ctrl + V
    private JMenuItem mi_Edict_Cut = new JMenuItem("Cut(X)");    //Ctrl + X
    //private JMenuItem mi_Edict_Select_All = new JMenuItem("SelectAll(A)"); //Ctrl + A

    private JMenu menu_Format_Auto_Wrap = new JMenu("AutoWrap");
    private JRadioButtonMenuItem rb_mi_Format_Auto_Wrap_on = new JRadioButtonMenuItem("On");
    private JRadioButtonMenuItem rb_mi_Format_Auto_Wrap_off = new JRadioButtonMenuItem("Off");
    private JMenu menu_Format_Tab_Size = new JMenu("TABSpace");
    private JRadioButtonMenuItem rb_mi_Format_Tab_Size_4 = new JRadioButtonMenuItem("TAB=4Space");
    private JRadioButtonMenuItem rb_mi_Format_Tab_Size_8 = new JRadioButtonMenuItem("TAB=8Space");

    private JMenuItem mi_Help_Help = new JMenuItem("Help(H)");        //Ctrl + H
    private JMenuItem mi_Help_About = new JMenuItem("About(A)");

    NoteBookMain() {
        frame.setTitle("Easy NoteBook");
        frame.setBounds(250, 200, 1000, 600);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
        frame.setResizable(false);
        initialize();
    }

    private void initialize(){
        menu_File.setMnemonic(KeyEvent.VK_F);
        menu_Edit.setMnemonic(KeyEvent.VK_E);
        menu_Format.setMnemonic(KeyEvent.VK_O);
        menu_Help.setMnemonic(KeyEvent.VK_H);

        mi_File_Save_As.setMnemonic(KeyEvent.VK_A);
        mi_File_Exit.setMnemonic(KeyEvent.VK_X);

        mi_File_New.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, KeyEvent.CTRL_DOWN_MASK, true));
        mi_File_Open.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, KeyEvent.CTRL_DOWN_MASK, true));
        mi_File_Save.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, KeyEvent.CTRL_DOWN_MASK, true));

        //mi_Edict_Undo.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Z, KeyEvent.CTRL_DOWN_MASK, true));
        mi_Edict_Copy.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, KeyEvent.CTRL_DOWN_MASK, true));
        mi_Edict_Paste.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_V, KeyEvent.CTRL_DOWN_MASK, true));
        mi_Edict_Cut.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X, KeyEvent.CTRL_DOWN_MASK, true));
        //mi_Edict_Select_All.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, KeyEvent.CTRL_DOWN_MASK, true));

        mi_Help_Help.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_H, KeyEvent.CTRL_DOWN_MASK, true));

        menu_File.add(mi_File_New);
        menu_File.add(mi_File_Open);
        menu_File.add(mi_File_Save);
        menu_File.add(mi_File_Save_As);
        menu_File.addSeparator();
        menu_File.add(mi_File_Exit);

        //menu_Edit.add(mi_Edict_Undo);
        menu_Edit.addSeparator();
        menu_Edit.add(mi_Edict_Copy);
        menu_Edit.add(mi_Edict_Paste);
        menu_Edit.add(mi_Edict_Cut);

        menu_Format_Auto_Wrap.add(rb_mi_Format_Auto_Wrap_on);
        menu_Format_Auto_Wrap.add(rb_mi_Format_Auto_Wrap_off);
        rb_mi_Format_Auto_Wrap_on.setSelected(true);
        textArea.setLineWrap(true);
        menu_Format.add(menu_Format_Auto_Wrap);
        menu_Format_Tab_Size.add(rb_mi_Format_Tab_Size_4);
        menu_Format_Tab_Size.add(rb_mi_Format_Tab_Size_8);
        rb_mi_Format_Tab_Size_4.setSelected(true);
        textArea.setTabSize(4);
        menu_Format.add(menu_Format_Tab_Size);

        menu_Help.add(mi_Help_Help);
        menu_Help.addSeparator();
        menu_Help.add(mi_Help_About);

        menuBar.add(menu_File);
        menuBar.add(menu_Edit);
        menuBar.add(menu_Format);
        menuBar.add(menu_Help);

        frame.setJMenuBar(menuBar);
        frame.add(scrollPane, BorderLayout.CENTER);
        textArea.setBounds(frame.getX() + 50, frame.getY() + 50, frame.getWidth(), 0);
        scrollPane.setBounds(frame.getX() + 50, frame.getY() + 50, frame.getWidth(), 0);

        mi_File_New.addActionListener(new NoteBookMain.ActionHandler());
        mi_File_Open.addActionListener(new NoteBookMain.ActionHandler());
        mi_File_Save.addActionListener(new NoteBookMain.ActionHandler());
        mi_File_Save_As.addActionListener(new NoteBookMain.ActionHandler());
        mi_File_Exit.addActionListener(new NoteBookMain.ActionHandler());

        //mi_Edict_Undo.addActionListener(new NoteBookMain.ActionHandler());
        mi_Edict_Copy.addActionListener(new NoteBookMain.ActionHandler());
        mi_Edict_Paste.addActionListener(new NoteBookMain.ActionHandler());
        mi_Edict_Cut.addActionListener(new NoteBookMain.ActionHandler());

        rb_mi_Format_Auto_Wrap_on.addActionListener(new NoteBookMain.ActionHandler());
        rb_mi_Format_Auto_Wrap_off.addActionListener(new NoteBookMain.ActionHandler());
        rb_mi_Format_Tab_Size_4.addActionListener(new NoteBookMain.ActionHandler());
        rb_mi_Format_Tab_Size_8.addActionListener(new NoteBookMain.ActionHandler());

        mi_Help_Help.addActionListener(new NoteBookMain.ActionHandler());
        mi_Help_About.addActionListener(new NoteBookMain.ActionHandler());
    }

    class ActionHandler implements ActionListener{

        @Override
        public void actionPerformed(ActionEvent e) {
            if(e.getSource() == mi_File_New){
                new NoteBookFile().NewFile(textArea, frame);
            }
            else if(e.getSource() == mi_File_Open){
                new NoteBookFile().OpenFile(textArea, frame);
            }
            else if(e.getSource() == mi_File_Save){
                new NoteBookFile().SaveFile(textArea, frame);
            }
            else if(e.getSource() == mi_File_Save_As){
                new NoteBookFile().SaveAsFile(textArea, frame);
            }
            else if(e.getSource() == mi_File_Exit){
                System.exit(0);
            }/*
            else if(e.getSource() == mi_Edict_Undo){
                //Undo
            }*/
            else if(e.getSource() == mi_Edict_Copy){
                textArea.copy();
            }
            else if(e.getSource() == mi_Edict_Paste){
                textArea.paste();
            }
            else if(e.getSource() == mi_Edict_Cut){
                textArea.cut();
            }
            else if(e.getSource() == rb_mi_Format_Auto_Wrap_on){
                if(rb_mi_Format_Auto_Wrap_off.isSelected()){
                    rb_mi_Format_Auto_Wrap_off.setSelected(false);
                }
                textArea.setLineWrap(true);
            }
            else if(e.getSource() == rb_mi_Format_Auto_Wrap_off){
                if(rb_mi_Format_Auto_Wrap_on.isSelected()){
                    rb_mi_Format_Auto_Wrap_on.setSelected(false);
                }
                textArea.setLineWrap(false);
            }
            else if(e.getSource() == rb_mi_Format_Tab_Size_4){
                if(rb_mi_Format_Tab_Size_8.isSelected()){
                    rb_mi_Format_Tab_Size_8.setSelected(false);
                }
                textArea.setTabSize(4);
            }
            else if(e.getSource() == rb_mi_Format_Tab_Size_8){
                if(rb_mi_Format_Tab_Size_4.isSelected()){
                    rb_mi_Format_Tab_Size_4.setSelected(false);
                }
                textArea.setTabSize(8);
            }
            else if(e.getSource() == mi_Help_Help){
                JOptionPane.showMessageDialog(frame, "Nothing in here");
            }
            else if(e.getSource() == mi_Help_About){
                JOptionPane.showMessageDialog(frame, "Power By NiceT\n");
            }
        }
    }
}
