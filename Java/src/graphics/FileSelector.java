package graphics;
import javax.swing.JButton;
import mintools.swing.FileSelect;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

public abstract class FileSelector extends JButton {
	protected abstract void selected(String file) throws Exception;
	
	public FileSelector(String label, final String directory, final String ... extensions) {
		setText(label);
		describe(extensions);
        addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				File file = FileSelect.select(extensions, description, "load", directory, true);
				if (file != null)
					try {
						selected(file.getPath());
					} catch (Exception e) { e.printStackTrace(); }
			}
        });
	}
	
	void describe(String[] extensions) {
		int i = 0;
		for (; i < extensions.length - 1; i++)
			description += ", " + extensions[i];
		
		description = description.substring(2) + " and " + extensions[i] + " files.";
	}
	
	String description = "";
}
