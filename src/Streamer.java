import java.io.*;
import java.net.*;
import java.util.*;

public class Streamer{

    public final static int MAX_PORT = 9999;
    public final static int MAX_NUM_MESS = 9999;
    public final static int SIZE_DIFF_MESS = Message.MSG_TYPE_SIZE + Message.MSG_NUM_MESS_SIZE + Message.MSG_ID_SIZE + Message.MSG_MESS_SIZE + 5;

    private String id;
    private int recvPort;
    private String multiCastAddr;
    private int multiCastPort;
    private int frequency;

    private ArrayList<String> diffMess = new ArrayList<String>();
    private ArrayList<String> IDs = new ArrayList<String>();
    private int current = 0;
    private int num = 0;

    private byte[][] lastStreamed = new byte[1000][SIZE_DIFF_MESS];
    private int lastSend = 0;

    public void initLastStreamed(){
        for(int i = 0; i < this.lastStreamed.length; i++) this.lastStreamed[i] = null;
    }

    public Streamer(String _id, int _recvPort, String _multiCastAddr, int _multiCastPort, int _frequency){
        this.id = _id;
        this. recvPort = _recvPort;
        this.multiCastAddr = _multiCastAddr;
        this.multiCastPort = _multiCastPort;
        this.frequency = _frequency;
        this.initLastStreamed();
    }



    /*******************
     *Getters & setters*
     ******************/

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

    public ArrayList<String> getDiffMess(){
        return this.diffMess;
    }

    public int getCurrent(){
        return this.current;
    }

    public void setCurrent(int _current){
        this.current = _current;
    }

    public int getFrequency(){
        return this.frequency;
    }

    public int getLastSend(){
        return this.lastSend;
    }

    public void setLastSend(int _lastStreamed){
        this.lastSend = _lastStreamed;
    }

    public int getNum(){
        return this.num;
    }

    public void setNum(int n){
        this.num = n;
    }

    public ArrayList<String> getIDs(){
        return this.IDs;
    }

    /***********************
     *End getters & setters*
     **********************/






    /* *********************************
     *Function to interact with clients*
     **********************************/

    public void addMess(String msg, String idMess){
        this.diffMess.add(msg);
        this.IDs.add(idMess);
    }

    public int getNbMess(){
        return this.diffMess.size();
    }

    public String getMess(int index){
        return this.diffMess.get(index);
    }

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

    public byte[][] getLastMess(int nb){
        int nbEl = nbElements(this.lastStreamed);
        if(nb > nbEl){
            byte[][] ret = new byte[nbEl][SIZE_DIFF_MESS];
            for(int i = 0; i < nbEl; i++) ret[i] = this.lastStreamed[i];
            return ret;
        }
        else{
            byte[][] ret = new byte[nb][SIZE_DIFF_MESS];
            if(nb > this.lastSend){
                byte[][] arr = java.util.Arrays.copyOfRange(this.lastStreamed, 0, this.lastSend);
                byte[][] arr2 = java.util.Arrays.copyOfRange(this.lastStreamed, nbEl - (nb - this.lastSend), nbEl);
                for(int i = 0; i < arr2.length; i++) ret[i] = arr2[i];
                for(int i = arr2.length; i < arr.length; i++) ret[i] = arr[i];
                return ret;
            }
            else{
                for(int i = this.lastSend - nb; i < this.lastSend; i++) ret[i - (this.lastSend - nb)] = this.lastStreamed[i];
                return ret;
            }
        }
    }

    /*************************************** 
     *End functions to interact with client*
     **************************************/





    /************************************
     *Functions to interact with manager*
     ***********************************/

    public static String getStreamerAddress(){
        try{
            InetAddress myIA=InetAddress.getLocalHost();
            return myIA.getHostAddress();
        } 
        catch(Exception e){
            e.printStackTrace();
            return null;
        }
    }

    /***************************************
    *End functions to interact with manager*
    ***************************************/





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
        Streamer s = new Streamer("streamer", 4242, "225.0.0.0", 5001, 1000);
        s.addMess("hellos", s.id);
        s.addMess("baiana", s.id);
        s.addMess("hellllllooooooooo wtf", s.id);
        startStream(s);
        
    }
    
}
