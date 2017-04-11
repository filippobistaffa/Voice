import nl.uu.cs.treewidth.input.GraphInput.InputData;
import nl.uu.cs.treewidth.timing.JavaNanoTime;
import nl.uu.cs.treewidth.output.NeatoViewer;
import nl.uu.cs.treewidth.output.DotWriter;
import nl.uu.cs.treewidth.timing.Stopwatch;
import nl.uu.cs.treewidth.algorithm.*;
import nl.uu.cs.treewidth.ngraph.*;
import java.util.stream.Stream;
import java.util.Collections;
import java.util.Comparator;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Arrays;
import java.awt.Point;
import java.util.Set;

public class TD {

	static private void process(String line, NGraph<InputData> g, ArrayList<NVertex<InputData>> array) {

		int nspaces = line.length() - line.replace(" ", "").length();

		if (nspaces == 0) {

			int id = array.size();
			NVertex<InputData> v = new ListVertex<InputData>();
			v.data = new InputData();
			v.data.id = id;
			v.data.name = Integer.toString(id);
			array.add(v);
			g.addVertex(v);

		} else {

			String[] parts = line.split(" ");
			int v1id = Integer.parseInt(parts[0].replaceAll("\\*", ""));
			int v2id = Integer.parseInt(parts[1].replaceAll("\\*", ""));
			g.addEdge(array.get(v1id), array.get(v2id));
		}
	}

	static public void main(String arg[]) throws java.io.IOException {

		// Read input graph in NGraph format

		NGraph<InputData> g = new ListGraph<InputData>();
		ArrayList<NVertex<InputData>> array = new ArrayList<NVertex<InputData>>();

		try (Stream<String> lines = java.nio.file.Files.lines(java.nio.file.Paths.get(arg[0]))) {
			lines.forEachOrdered(line -> process(line, g, array));
		}

		Stopwatch stopwatch = new Stopwatch(new JavaNanoTime());

		// Heuristc

		//GreedyDegree<InputData> ub = new GreedyDegree<InputData>();
		GreedyFillIn<InputData> ub = new GreedyFillIn<InputData>();
		stopwatch.reset();
		stopwatch.start();
		ub.setInput(g);
		ub.run();
		stopwatch.stop();
		if (arg.length > 2) { // CSV output
			System.out.print(ub.getUpperBound() + ",");
		} else {
			System.out.println("Heuristic : " + ub.getName());
			System.out.println("Treewidth : " + ub.getUpperBound());
			System.out.println("Runtime   : " + stopwatch.getTime() + " ms");
		}
		NVertexOrder<InputData> order = ub.getPermutation();

		// Exact (QuickBB)

		/*

		QuickBB<InputData> qbb = new QuickBB<InputData>();
		stopwatch.reset();
		stopwatch.start();
		qbb.setInput(g);
		qbb.run();
		stopwatch.stop();
		System.out.println("Exact     : " + qbb.getName());
		System.out.println("Treewidth : " + qbb.getUpperBound());
		System.out.println("Runtime   : " + stopwatch.getTime() + " ms");
		//System.out.println("");

		*/

		// Exact (TreewidthDP)

		/*

		TreewidthDP<InputData> twdp = new TreewidthDP<InputData>(ub.getUpperBound());
		stopwatch.reset();
		stopwatch.start();
		twdp.setInput(g);
		twdp.run();
		stopwatch.stop();
		System.out.println("Exact     : " + twdp.getName());
		System.out.println("Treewidth : " + twdp.getTreewidth());
		System.out.println("Runtime   : " + stopwatch.getTime() + " ms");

		*/

		PermutationToTreeDecomposition<InputData> ptd = new PermutationToTreeDecomposition<InputData>(order);
		ptd.setInput(g);
		ptd.run();
		NGraph<NTDBag<InputData>> td = ptd.getDecomposition();
		ArrayList<NVertex<NTDBag<InputData>>> tdvertices = new ArrayList<NVertex<NTDBag<InputData>>>(td.getNumberOfVertices());
		ArrayList<Point> points = new ArrayList<Point>(td.getNumberOfVertices());

		//String dotg = DotWriter.format(g);
		//NeatoViewer.present(dotg, "", 0, 0, false, true);

		//String dottd = DotWriter.formatTD(td);
		//NeatoViewer.present(dottd, "", 500, 0, false, true);

		int i = 0;
		for (Iterator<NVertex<NTDBag<InputData>>> it = td.getVertices(); it.hasNext();) {
			NVertex<NTDBag<InputData>> ntdv = it.next();
			tdvertices.add(ntdv);
			points.add(new Point(i++, 0));
		}

		i = 0;
		for (Iterator<NVertex<NTDBag<InputData>>> it = td.getVertices(); it.hasNext();) {
			NVertex<NTDBag<InputData>> ntdv = it.next();
			for (Iterator<NVertex<NTDBag<InputData>>> itn = ntdv.getNeighbors(); itn.hasNext();) {
				NVertex<NTDBag<InputData>> neigh = itn.next();
				int j = tdvertices.indexOf(neigh);
				if (j > i) points.get(j).y = points.get(i).y + 1;
			}
			i++;
		}

		Collections.sort(points, new Comparator<Point>() { 
			public int compare(Point o1, Point o2) { return Integer.compare(o1.y, o2.y); }
		});

		ArrayList<NVertex<NTDBag<InputData>>> tdvo = new ArrayList<NVertex<NTDBag<InputData>>>();

		for (i = 0; i < points.size(); i++)
			tdvo.add(tdvertices.get(points.get(i).x));

		PrintWriter writer = new PrintWriter(arg[1], "UTF-8");
		writer.println(td.getNumberOfVertices());

		for (NVertex<NTDBag<InputData>> ntdv : tdvo) {
			Set<NVertex<InputData>> set = ntdv.data.vertices;
			Integer[] buf = new Integer[set.size()];
			i = 0;
			for (NVertex<InputData> v : set) buf[i++] = v.data.id;
			String str = Arrays.toString(buf).replace(",", "");
			writer.println(str.substring(1, str.length() - 1));
		}

		i = 0;
		for (NVertex<NTDBag<InputData>> ntdv : tdvo) {
			int desc = 0;
			for (Iterator<NVertex<NTDBag<InputData>>> itn = ntdv.getNeighbors(); itn.hasNext();)
				if (tdvo.indexOf(itn.next()) > i)
					desc++;
			Integer[] buf = new Integer[desc];
			int j = 0;
			for (Iterator<NVertex<NTDBag<InputData>>> itn = ntdv.getNeighbors(); itn.hasNext();) {
				NVertex<NTDBag<InputData>> neigh = itn.next();
				int k = tdvo.indexOf(neigh);
				if (k > i) buf[j++] = k;
			}
			String str = Arrays.toString(buf).replace(",", "");
			writer.println(str.substring(1, str.length() - 1));
			i++;
		}

		writer.close();
	}
}
