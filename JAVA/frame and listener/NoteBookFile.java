import javax.swing.*;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

class NoteBookFile {
    private JFileChooser fileChooser = new JFileChooser();
    private File file;

    void NewFile(JTextArea textArea, JFrame frame) {
        if(!textArea.getText().equals("")){
            SaveFile(textArea, frame);
        }
        textArea.setText(null);
        file = null;
        frame.setTitle("Easy NoteBook");
    }

    void OpenFile(JTextArea textArea, JFrame frame) {
        if(!textArea.getText().equals("")){
            SaveFile(textArea, frame);
        }
        int option = fileChooser.showOpenDialog(frame);
        if(option == JFileChooser.APPROVE_OPTION){
            file = fileChooser.getSelectedFile();
            try{
                FileReader fileReader = new FileReader(file);
                textArea.read(fileReader, null);
                frame.setTitle(file.getName() + " - Easy NoteBook");
                fileReader.close();
            }
            catch (IOException e){
                JOptionPane.showMessageDialog(frame, "Exception: \n" + e.getMessage());
            }
        }
    }

    void SaveFile(JTextArea textArea, JFrame frame) {
        if(file != null && file.exists()){
            try{
                FileWriter fileWriter = new FileWriter(file);
                textArea.write(fileWriter);
                fileWriter.close();
            }
            catch (IOException e){
                JOptionPane.showMessageDialog(frame, "Exception: \n" + e.getMessage());
            }
        }
        else {
            SaveAsFile(textArea, frame);
        }
    }

    void SaveAsFile(JTextArea textArea, JFrame frame) {
        int option = fileChooser.showSaveDialog(frame);
        if(option == JFileChooser.APPROVE_OPTION){
            file = fileChooser.getSelectedFile();
            try {
                FileWriter fileWriter = new FileWriter(file);
                textArea.write(fileWriter);
                frame.setTitle(file.getName() + " - Easy NoteBook");
                fileWriter.close();
            }
            catch (IOException e){
                JOptionPane.showMessageDialog(frame, "Exception: \n" + e.getMessage());
            }
        }
    }
}
