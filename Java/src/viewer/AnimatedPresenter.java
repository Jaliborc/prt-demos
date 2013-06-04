package viewer;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import mintools.parameters.BooleanParameter;
import mintools.parameters.IntParameter;

import org.lwjgl.opengl.Drawable;

import graphics.FileSelector;
import graphics.Presenter;

class AnimatedPresenter extends Presenter {
	public void startup(final Drawable drawable) throws Exception {
		scene = new Scene("../Ambients/CornellBox.tga");
		loop = new BooleanParameter("Loop", false);
		drawScene = new BooleanParameter("Draw Scene", true);
		pose =  new IntParameter("Pose", 0, 0, numPoses - 1);
		
		new Timer().schedule(new TimerTask() {
			public void run() {
				pose.setValue(pose.getValue() + 1);
				render(pose.getValue());
				
		}}, new Date(), 300);
		
		add(new FileSelector("Scene", "../Ambients/", "tga", "png") {
			protected void selected(String file) throws Exception {
				drawable.makeCurrent();
				scene = new Scene(file);
		}});
		
		add(loop.getControls());
		add(drawScene.getControls());
		add(pose.getSliderControls());
	}
	
	public void render(int pose) {}
	public void draw() {
		if (drawScene.getValue())
			scene.draw(1);
	}
	
	Scene scene;
	BooleanParameter loop, drawScene;
	IntParameter pose;
	int numPoses;
}
