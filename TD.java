import nl.uu.cs.treewidth.input.GraphInput.InputData;
import nl.uu.cs.treewidth.timing.JavaNanoTime;
import nl.uu.cs.treewidth.output.NeatoViewer;
import nl.uu.cs.treewidth.output.DotWriter;
import nl.uu.cs.treewidth.timing.Stopwatch;
import nl.uu.cs.treewidth.algorithm.*;
import nl.uu.cs.treewidth.ngraph.*;
import java.util.stream.Stream;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Arrays;
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
		System.out.println("Heuristic : " + ub.getName());
		System.out.println("Treewidth : " + ub.getUpperBound());
		System.out.println("Runtime   : " + stopwatch.getTime() + " ms");
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
		ArrayList<NTDBag<InputData>> tdvertices = new ArrayList<NTDBag<InputData>>();
		PrintWriter writer = new PrintWriter(arg[1], "UTF-8");
		writer.println(td.getNumberOfVertices());

		for (Iterator<NVertex<NTDBag<InputData>>> it = td.getVertices(); it.hasNext();) {
			NTDBag<InputData> ntdb = it.next().data;
			tdvertices.add(ntdb);
			Set<NVertex<InputData>> set = ntdb.vertices;
			Integer[] buf = new Integer[set.size()];
			int i = 0;
			for (NVertex<InputData> v : set) buf[i++] = v.data.id;
			String str = Arrays.toString(buf).replace(",", "");
			writer.println(str.substring(1, str.length() - 1));
		}

		for (Iterator<NVertex<NTDBag<InputData>>> it = td.getVertices(); it.hasNext();) {
			NVertex<NTDBag<InputData>> ntdv = it.next();
			Integer[] buf = new Integer[ntdv.getNumberOfNeighbors()];
			int i = 0;
			for (Iterator<NVertex<NTDBag<InputData>>> itn = ntdv.getNeighbors(); itn.hasNext();) {
				NVertex<NTDBag<InputData>> neigh = itn.next();
				buf[i++] = tdvertices.indexOf(neigh.data);
			}
			String str = Arrays.toString(buf).replace(",", "");
			writer.println(str.substring(1, str.length() - 1));
		}

		writer.close();

		//String dotg = DotWriter.format(g);
		//NeatoViewer.present(dotg, "", 0, 0, false, true);

		//String dottd = DotWriter.formatTD(td);
		//NeatoViewer.present(dottd, "", 500, 0, false, true);
	}
}
