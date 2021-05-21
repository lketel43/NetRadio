import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;

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

    private ArrayList<byte[]> images = new ArrayList<byte[]>();
    private int currentImage = 0;
    private int imageFrequency = 5000;
    public String multiCastAddrImages = "225.0.0.1";
    public int multiCastPortImages = 5002;

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

    public ArrayList<byte[]> getImages(){
        return this.images;
    }

    public int getCurrentImage(){
        return this.currentImage;
    }

    public void setCurrentImage(int n){
        this.currentImage = n;
    }

    public int getImageFrequency(){
        return this.imageFrequency;
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





    /**********************************
    *Functions to display informations*
    **********************************/

    private static String display(String s, int length){
        StringBuffer buf = new StringBuffer(length);
        for(int i = 0; i < s.length(); i++) buf.append(s.charAt(i));
        for(int i = 0; i < length - s.length() - 1; i++) buf.append(" ");
        buf.append("#");
        return new String(buf);
    }

    private static void displayClientInfo(Socket sock){
        System.out.println("#######################################\n" +
                            "# Nouvelle connexion avec etablie     #" + "\n" +
                            display("# Port local : " + Integer.toString(sock.getLocalPort()), 39) + "\n" +
                            display("# Port distant : " + Integer.toString(sock.getPort ()), 39) + "\n" +
                            display("# Adresse locale : " + sock.getLocalAddress(), 39) + "\n" +
                            display("# Adresse distante : " + sock.getInetAddress(), 39) + "\n" +
                            "#######################################\n"
                            );
    }

    public static void displayMessAdded(String id){
        System.out.println("###########################################\n" +
                            display("# Un message a ete diffuse par : " + id, 43) + "\n" +
                            "###########################################\n"
                          );
    }

    public static void displayLastMess(int nbMess){
        System.out.println("##########################################################################\n" +
                            display("# Un utilisateur a demande a consulter les " + Integer.toString(nbMess) + " derniers messages diffuses", 74) + "\n" +
                            "##########################################################################\n"
                            );
    }

    /**************************************
    *End functions to display informations*
    **************************************/

     /*****************************
    * Functions to manage images *
    *****************************/

    public int getNbImages(){
        return this.images.size();
    }

    public byte[] getImage(int c){
        return this.images.get(c);
    }

    public void addImage(String imgPath){
        try{
            BufferedImage image = ImageIO.read(new File(imgPath));
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            ImageIO.write(image, "png", baos);
            baos.flush();
            byte[] buffer = baos.toByteArray();
            this.images.add(buffer);
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

    /*********************************
    * End Functions to manage images *
    *********************************/



    public static void startStream(Streamer stream, String managerAddress, int managerPort, boolean castImages){
        MulticastService mService = new MulticastService(stream);
        Thread mThread = new Thread(mService);
        mThread.start();
        if(managerAddress != null && managerPort != -1){
            ManagerService manService = new ManagerService(stream, managerPort, managerAddress);
            Thread rThread = new Thread(manService);
            rThread.start();
        }
        if(stream.getNbImages() > 0 && castImages){
            ImageMulticastService imService = new ImageMulticastService(stream);
            Thread imThread = new Thread(imService);
            imThread.start();
        }
        try{
            ServerSocket server = new ServerSocket(stream.recvPort);
            while(true){
                Socket sock = server.accept();
                displayClientInfo(sock);
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
        if(args.length == 0){
            int defRecvPort = 4242;
            String defAddr = "225.0.0.0";
            String defID = "STREAMER";
            int defFrequency = 1000;
            int defMultiPort = 5001;
            Streamer s = new Streamer(defID, defRecvPort, defAddr, defMultiPort, defFrequency);
            s.addMess("first message", defID);
            s.addMess("second message", defID);
            s.addMess("third message", defID);
            s.addMess("forth message", defID);
            s.addImage("./java/images/i.png");
            s.addImage("./java/images/icon.png");
            s.addImage("./java/images/im2.png");
            s.addImage("./java/images/im3.png");
            s.addImage("./java/images/im4.png");
            startStream(s, null, -1, true);
        }
        else if(args.length == 5){
            Streamer s = new Streamer(args[0], Integer.parseInt(args[1]), args[2], Integer.parseInt(args[3]), Integer.parseInt(args[4]));
            s.addMess("first message", args[0]);
            s.addMess("second message", args[0]);
            s.addMess("third message", args[0]);
            s.addMess("forth message", args[0]);
            startStream(s, null, -1, false);
        }
        else if(args.length == 7){
            Streamer s = new Streamer(args[0], Integer.parseInt(args[1]), args[2], Integer.parseInt(args[3]), Integer.parseInt(args[4]));
            s.addMess("first message", args[0]);
            s.addMess("second message", args[0]);
            s.addMess("third message", args[0]);
            s.addMess("forth message", args[0]);
            startStream(s, args[5], Integer.parseInt(args[6]), false);
        }
        else if(args.length == 2){
            Streamer s = StreamFile.initStreamerFromFile(args[0]);
            StreamFile.addMessFromFile(s, args[1]);
            startStream(s, null, -1, false);
        }
        else System.out.println("Wrong options");
    }
    
}
