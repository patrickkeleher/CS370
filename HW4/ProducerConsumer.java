// Author Patrick Keleher
// Solution in Java to the producer/consumer problem
import java.util.Random;
public class ProducerConsumer {
	int tcounter = 0;
	double[] Buffer = new double[1000];
	// Consumer
	public synchronized double reduceBuffer(int output) {
		while(tcounter == 0) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		double bufferatIndex = Buffer[output];
		tcounter--;
		// Alert notify
		notify();
		return bufferatIndex;
	}
	//Producer
	public synchronized void bigBuffer(double bufferatIndex, int input){

		while(tcounter == 1000) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		Buffer[input] = bufferatIndex;
		// Increment Thread counter
		tcounter++;
		//Alert Notify
		notify();
	}
	public static void main(String[] args) {
		ProducerConsumer check = new ProducerConsumer();
		Thread giver = new Thread(new Runnable() {
			public void run() {
				double ptotal = 0.0;
				int input = 0;
				int rcount=1;
				while (rcount<=1000000) {
					Random rando = new Random();
					double bufferat = rando.nextDouble() * 100.0;
					ptotal += bufferat;
					check.bigBuffer(bufferat, input);
					input = (input + 1) % 1000;

					if (rcount != 0 && (rcount % 100000) == 0){
						System.out.printf("Producer: Generated %,d items, Cumulative value of generated items=%.3f\n", rcount, ptotal);
					}
					// Increment counter
					 rcount++;}
				System.out.println("Producer: Finished generating 1,000,000 items");
			}
		});

		Thread taker = new Thread(new Runnable() {


			public void run() {
				double total = 0.0;
				int out = 0;
				int rcount = 1;
				while (rcount<=1000000) {

					total += check.reduceBuffer(out);
					out = (out+1)%1000;

					if (rcount!=0 && (rcount%100000)== 0){
						System.out.printf("Consumer: Consumed %,d items, Cumulative value of consumed items=%.3f\n", rcount, total);
				}
					rcount++;}

				System.out.println("Consumer: Finished consuming 1,000,000 items");
			}
		});
		// Start the threads
		giver.start();
		taker.start();
		try {
			taker.join();
			giver.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		// Program is complete
		System.out.println("Exiting!");
	}
}
