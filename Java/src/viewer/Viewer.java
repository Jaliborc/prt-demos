package viewer;
import org.lwjgl.opengl.Drawable;
import graphics.Presenter;
import graphics.Window;

public class Viewer extends Window {
	public static void main(String[] args) throws Exception {
		Viewer viewer = new Viewer();
		viewer.extensions = new String[] {"obj", "pgv", "pgt", "pgst"};
		viewer.screenshotDirectory = "../Screenshots";
		viewer.modelDirectory = "../Models";
		viewer.Start("hand.pgv");
	}
	
	protected Presenter getPresenter(Drawable drawable, String file) {
		try {
			if (file.endsWith(".obj"))
				return new Obj(file);
			if (file.endsWith(".pgv"))
				return new Pgv(file);
			
			return new Pgt(drawable, file);
			
		} catch (Exception e) {e.printStackTrace();}
		return null;
	}
}
