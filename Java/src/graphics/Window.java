package graphics;
import static java.lang.Math.cos;
import static java.lang.Math.sin;

import mintools.swing.VerticalFlowPanel;
import javax.swing.JFrame;

import static java.awt.BorderLayout.*;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentAdapter;
import java.awt.Canvas;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

import static org.lwjgl.input.Keyboard.*;
import static org.lwjgl.util.glu.GLU.*;
import static org.lwjgl.opengl.GL11.*;

import org.lwjgl.BufferUtils;
import org.lwjgl.LWJGLException;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.Drawable;
import org.lwjgl.opengl.SharedDrawable;
import org.lwjgl.input.Keyboard;
import org.lwjgl.input.Mouse;

public abstract class Window extends JFrame {
	protected abstract Presenter getPresenter(Drawable surface, String file);
	public String screenshotDirectory;
	public String modelDirectory;
	public String[] extensions;
	
	public void Start(String file) throws Exception {
		this.file = modelDirectory + "/" + file;
		
		initWindow();
		initControls();
		initCanvas();
		initGL();
		
		while (isRunning())
			update();
		
		exit();
	}
		
	void initWindow() {
		addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent e) {
                resize();
            }
		});
		
		setSize(1280, 720);
		add(controls.getPanel(), EAST);
		add(canvas, WEST);
		setVisible(true);
	}
	
	void initControls() {
		browser = new FileSelector("Model", modelDirectory, extensions) {
			protected void selected(String target) {
				file = target;
			}
		};
	}
	
	void initCanvas() throws Exception {
		canvas.setIgnoreRepaint(true);
		canvas.setFocusable(true);
		
        Display.setResizable(true);  
        Display.setParent(canvas);
        Display.create();
        
        drawable = new SharedDrawable(Display.getDrawable());
	}
	
	void initGL() {
        glClearColor(0, 0, 0, 0);
        glClearDepth(1);
        
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
      	  
      	glMatrixMode(GL_MODELVIEW);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	}
	
	void update() throws Exception {
		if (file != null)
			loadPresenter();
		
		input();
		draw();
	}
	
	void loadPresenter() {
		if (presenter != null)
			presenter.getPanel().setVisible(false);
		
		presenter = getPresenter(drawable, file);
		controls.removeAll();
		controls.add(browser);
		controls.add(presenter.getPanel());
		file = null;
	}
	
	void input() {
		z += Mouse.getDWheel() * .0005;
		x += isKeyDown(KEY_LEFT) ? .01 : 0;
		x -= isKeyDown(KEY_RIGHT) ? .01 : 0;
		y += isKeyDown(KEY_DOWN) ? .01 : 0;
		y -= isKeyDown(KEY_UP) ? .01 : 0;
		
		while (Keyboard.next())
			if (getEventKey() == KEY_SPACE && getEventKeyState())
				screenshot();
	}
	
	void screenshot() {
		glReadBuffer(GL_FRONT);
		ByteBuffer buffer = BufferUtils.createByteBuffer(canvas.getWidth() * canvas.getHeight() * 3);
		glReadPixels(0, 0, canvas.getWidth(), canvas.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, buffer);

		try {
			FileChannel binary = new FileOutputStream(new File(screenshotDirectory + "/untitled")).getChannel();
			binary.write(buffer);
			binary.close();
		} catch (Exception e) {e.printStackTrace();}
	}
	
	void draw() throws Exception {
		if (resize)
			Display.setDisplayMode(new DisplayMode(canvas.getWidth(), canvas.getHeight()));
		
    	glMatrixMode(GL_PROJECTION);
      	glLoadIdentity();
      	gluPerspective(45, (float) canvas.getWidth()/canvas.getHeight(), 0.1f, 100);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(
			(float) (cos(x) * sin(y) * z), (float) cos(y ) * z, (float) (sin(x) * sin(y) * z),
			0, 0, 0,
			0, 1, 0);
		
		resize = false;
		presenter.draw();
		Display.update();
	}
	
	void resize() {
		controls.getPanel().setSize(550, getHeight());
		canvas.setSize(getWidth() - controls.getPanel().getWidth(), getHeight());
		resize = true;
	}
	
	boolean isRunning() {
		return !Display.isCloseRequested();
	}
	
	void exit() {
		Display.destroy();
		System.exit(0);
	}
	
	float z = 2;
	float x, y = 1;
	boolean resize;
	
	String file;
	Presenter presenter;
	
	Canvas canvas = new Canvas();
	Drawable drawable;
	
	VerticalFlowPanel controls = new VerticalFlowPanel();
	FileSelector browser;
}
