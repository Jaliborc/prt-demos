package exporters;
import java.io.*;

class IO {
	static String[] getFiles(String path, final String extension) {
		File target = new File(path);
		
		if (target.isDirectory()) {
			String[] files = target.list(new FilenameFilter() {
				public boolean accept(File dir, String file) {
					return file.endsWith(extension);
				}
			});
			
			for (int i = 0; i < files.length; i++)
				files[i] = path + "/" + files[i];
			
			return files;
		} else
			return new String[] {path};
	}
	
	static String removeExtension(String path) {
		return path.substring(0, path.lastIndexOf('.'));
	}
}
