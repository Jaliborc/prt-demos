package viewer;
import static java.lang.Float.parseFloat;
import static org.lwjgl.opengl.GL13.*;
import static org.lwjgl.util.glu.GLU.*;
import static org.lwjgl.opengl.GL11.*;

import math.Vector;

import org.lwjgl.util.glu.Sphere;

import org.newdawn.slick.opengl.TextureLoader;
import org.newdawn.slick.util.ResourceLoader;
import org.newdawn.slick.opengl.Texture;

import java.io.InputStreamReader;
import java.io.FileInputStream;
import java.io.BufferedReader;

class Scene extends Sphere {
	public float radiance[];
	public float directionalPower;
	public Vector directional;

	public Scene(String path) throws Exception {
		String extension = path.substring(path.lastIndexOf('.') + 1);
		String radiancePath = path.substring(0, path.lastIndexOf('.')) + ".radiance";
		String radianceSource = new BufferedReader(new InputStreamReader(new FileInputStream(radiancePath))).readLine();
		String[] values = radianceSource.split(" ");
		
		texture = TextureLoader.getTexture(extension, ResourceLoader.getResourceAsStream(path));
		radiance = new float[values.length];
		
		for (int i = 0; i < values.length; i++)
			radiance[i] = parseFloat(values[i]);

		directional = new Vector(-radiance[3], -radiance[1], radiance[2]);
		directional.normalise();
		directionalPower = 867f / 316 * (float) Math.PI *
				(directional.x * radiance[3] + directional.y * radiance[1] + directional.z * radiance[2]);
		
		if (directionalPower < 0) {
			directionalPower *= -1;
			directional.scale(-1);
		}
		
		setOrientation(GLU_INSIDE);
		setTextureFlag(true);
	}
	
	public void finalize() {
		texture.release();
	}
	
	public void draw() {
		glActiveTexture(GL_TEXTURE0);
		texture.bind();
	
		glColor3d(1,1,1);
		glPushMatrix();
		glRotatef(90f, 0,1,0);
		glRotatef(270f, 1,0,0);
		draw(50, 50, 50);
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	
	Texture texture;
}
