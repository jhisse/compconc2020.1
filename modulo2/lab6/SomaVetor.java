class Vetor {
    private int dimensao;
    private int[] vetor;

    public Vetor(int dimensao) {
        this.dimensao = dimensao;
        this.vetor = new int[dimensao];      
    }

    public void set(int posicao, int valor) {
        this.vetor[posicao] = valor;
    }

    public int get(int posicao) {
        return this.vetor[posicao];
    }

    public void imprime() {
        for(int i=0; i<this.dimensao; i++) {
            System.out.print(this.vetor[i] + " ");
        }
        System.out.println();
    }

    public int dimensao() {
        return this.dimensao;
    }

    public int[] vetor() {
        return this.vetor;
    }
    
    public void inicializa(int multiplo) {
        for(int i=0; i<this.dimensao; i++) {
            this.set(i, i*multiplo);
        }
    }
}


class Tarefa extends Thread {

    private int id;
    private int nThreads;

    private Vetor A;
    private Vetor B;
    private Vetor C;

    public Tarefa(int id, int nThreads, Vetor A, Vetor B, Vetor C){
        this.id = id;
        this.nThreads = nThreads;
        this.A = A;
        this.B = B;
        this.C = C;
    }

    public void run(){
        for (int i=this.id; i<A.dimensao(); i+=nThreads) {
            int soma = A.get(i) + B.get(i);
            C.set(i, soma);
        }
    }

}

public class SomaVetor{
    public static void main(String[] args){
        int nThreads;
        int dimensao;

        if(args.length < 2) {
            System.out.println("Execute com: java SomaVetor <quantidade de threads> <tamanho do vetor>");
            return;
        }
        
        nThreads = Integer.parseInt(args[0]);
        dimensao =  Integer.parseInt(args[1]);

        if(nThreads > dimensao){ nThreads=dimensao; }        

        Vetor A = new Vetor(dimensao);
        Vetor B = new Vetor(dimensao);
        Vetor C = new Vetor(dimensao);

        A.inicializa(2);
        System.out.println("Vetor A:");
        A.imprime();

        B.inicializa(3);
        System.out.println("Vetor B:");
        B.imprime();

        Thread[] threads = new Thread[nThreads];

        for (int i=0; i<nThreads; i++) {
            threads[i] = new Tarefa(i, nThreads, A, B, C);
        }

        for (int i=0; i<nThreads; i++) {
            threads[i].start();
        }

        for (int i=0; i<nThreads; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        System.out.println("Vetor C:");
        C.imprime();
  }
}
