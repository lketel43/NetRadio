import java.io.*;
import java.net.*;

public class Streamer{

    public final static int MAX_PORT = 9999;
    public final static int MAX_NUM_MESS = 9999;
    public final static int SIZE_DIFF_MESS = Message.MSG_TYPE_SIZE + Message.MSG_NUM_MESS_SIZE + Message.MSG_ID_SIZE + Message.MSG_MESS_SIZE + 5;

    private String id;
    private int recvPort;
    private String multiCastAddr;
    private int multiCastPort;
    private int frequency;

    private byte[][] diff_mess = new byte[MAX_NUM_MESS][SIZE_DIFF_MESS];
    private int lastMess = 0;
    private int current = 0;

    private byte[][] lastStreamed = new byte[1000][SIZE_DIFF_MESS];
    private int currentStreamed = 0;

    public void initDiffMess(){
        for(int i = 0; i < this.diff_mess.length; i++) this.diff_mess[i] = null;
    }

    public void initLastStreamed(){
        for(int i = 0; i < this.lastStreamed.length; i++) this.lastStreamed[i] = null;
    }

    public Streamer(String _id, int _recvPort, String _multiCastAddr, int _multiCastPort, int _frequency){
        this.id = _id;
        this. recvPort = _recvPort;
        this.multiCastAddr = _multiCastAddr;
        this.multiCastPort = _multiCastPort;
        this.frequency = _frequency;
        this.initDiffMess();
        this.initLastStreamed();
    }




    // Getters & setters

    public String getId(){
        return this.id;
    }

    public int getRecvPort(){
        return this.recvPort;
    }

    public String getMultiCastAddr(){
        return this.multiCastAddr;
    }

    public int getMultiCastPort(){
        return this.multiCastPort;
    }

    public byte[][] getDiffMess(){
        return this.diff_mess;
    }

    public int getCurrent(){
        return this.current;
    }

    public void setCurrent(int _current){
        this.current = _current;
    }

    public int getLastMess(){
        return this.lastMess;
    }

    public int getFrequency(){
        return this.frequency;
    }

    public int getCurrentStreamed(){
        return this.currentStreamed;
    }

    public void setCurrentStreamed(int _lastStreamed){
        this.currentStreamed = _lastStreamed;
    }

    // End getters & setters






    public int nbElements(byte[][] arr){
        int nb = 0;
        for(int i = 0; i < arr.length; i++){
            if(arr[i] != null) nb++;
        }
        return nb;
    }

    public void addLast(byte[] mess, int index){
        this.lastStreamed[index] = mess;
    }

    public void addMess(byte[] mess){
        if(this.lastMess < MAX_NUM_MESS){
            this.diff_mess[this.lastMess] = mess;
            this.lastMess++;
        }
        else if(this.lastMess == MAX_NUM_MESS){
            this.lastMess = 0;
            this.diff_mess[0] = mess;
        }
    }

    public byte[][] getLastMess(int nb){
        int nbEl = nbElements(this.lastStreamed);
        if(nb > nbEl){
            byte[][] ret = new byte[nbEl][SIZE_DIFF_MESS];
            for(int i = 0; i < nbEl; i++) ret[i] = this.lastStreamed[i];
            return ret;
        }
        else{
            byte[][] ret = new byte[nb][SIZE_DIFF_MESS];
            if(nb > this.currentStreamed){
                byte[][] arr = java.util.Arrays.copyOfRange(this.lastStreamed, 0, this.currentStreamed);
                byte[][] arr2 = java.util.Arrays.copyOfRange(this.lastStreamed, nbEl - (nb - this.currentStreamed), nbEl);
                for(int i = 0; i < arr2.length; i++) ret[i] = arr2[i];
                for(int i = arr2.length; i < arr.length; i++) ret[i] = arr[i];
                return ret;
            }
            else{
                for(int i = this.currentStreamed - nb; i < this.currentStreamed; i++) ret[i - (this.currentStreamed - nb)] = this.lastStreamed[i];
                return ret;
            }
        }
    }

    public static void startStream(Streamer stream){
        MulticastService mService = new MulticastService(stream);
        Thread mThread = new Thread(mService);
        mThread.start();
        try{
            ServerSocket server = new ServerSocket(stream.recvPort);
            while(true){
                Socket sock = server.accept();
                ClientService cService = new ClientService(stream, sock);
                Thread cThread = new Thread(cService);
                cThread.start();
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

    public static void main(String[] args){
        
        byte[] msg1 = Message.createMsg("DIFF 0 RADIO Le monde est beau, je suis content j'ai rien a dire mais je veux parler quand meme nique les rageux");
        byte[] msg2 = Message.createMsg("DIFF 1 RADIO1 salut !");
        byte[] msg3 = Message.createMsg("DIFF 2 RADIO2 hello world !");
        byte[] msg4 = Message.createMsg("DIFF 3 RADIO3 baiana !");
        Streamer s = new Streamer("streamer", 4242, "225.0.0.0", 5001, 1000);
        s.addMess(msg2);
        s.addMess(msg3);
        s.addMess(msg4);
        startStream(s);
        //System.out.println(s.diff_mess.length);
    }
    
}
