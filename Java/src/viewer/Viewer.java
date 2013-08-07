package viewer;
import org.lwjgl.opengl.Drawable;

import graphics.Presenter;
import graphics.Window;

public class Viewer extends Window {
	public static void main(String[] args) throws Exception {
		Viewer viewer = new Viewer();
		viewer.extensions = new String[] {"obj", "pgv", "pgt", "pgst"};
		viewer.screenshotDirectory = "../Models";
		viewer.modelDirectory = "../Models";
		viewer.Start("octahedron.pgt");
	}
	
	protected Presenter getPresenter(Drawable drawable, String file) {
		try {
			if (file.endsWith(".pgt"))
				return new PgtPresenter(drawable, file);
			if (file.endsWith(".pgst"))
				return new PgstPresenter(drawable, file);
			
			if (file.endsWith(".pgv"))
				return new PgvPresenter(drawable, file);
			
			if (file.endsWith(".obj"))
				return new ObjPresenter(file);
			
		} catch (Exception e) {e.printStackTrace();}
		return null;
	}
}
