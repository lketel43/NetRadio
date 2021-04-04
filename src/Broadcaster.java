import java.io.*;
import java.net.*;

public class Broadcaster{

    public final static int MAX_PORT = 9999;
    public final static int MAX_NUM_MESS = 9999;
    public final static int SIZE_DIFF_MESS = Message.MSG_TYPE_SIZE + Message.MSG_NUM_MESS_SIZE + Message.MSG_ID_SIZE + Message.MSG_MESS_SIZE;

    private String id;
    private int recvPort;
    private String multiCastAddr;
    private int multiCastPort;
    private int frequency;

    private byte[][] diff_mess = new byte[SIZE_DIFF_MESS][MAX_NUM_MESS];
    private int current = 0;

    public Broadcaster(String _id, int _recvPort, String _multiCastAddr, int _multiCastPort, int _frequency){
        this.id = _id;
        this. recvPort = _recvPort;
        this.multiCastAddr = _multiCastAddr;
        this.multiCastPort = _multiCastPort;
        this.frequency = _frequency;
    }

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
    
}