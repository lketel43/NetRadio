

public class Message{
    // Taille des caractéristiques d'un message en octets
    static final int MSG_ID_SIZE = 8;
    static final int MSG_IP_SIZE = 15;
    static final int MSG_MESS_SIZE = 140;
    static final int MSG_NB_MESS = 3;
    static final int MSG_NUM_DIFF_SIZE = 2;
    static final int MSG_NUM_MESS_SIZE = 4;
    static final int MSG_PORT_SIZE = 4;
    static final int MSG_TYPE_SIZE = 4;

    static final String MSG_END = "\r\n";
    static final int MSG_END_SIZE = 2;

    static final char DEFAULT_CHAR = '#'; 
    static final char DEFAULT_INT = '0';

    // Types possibles des messages en octets
    public enum MsgType {
        ACKM,
        DIFF,
        ENDM,
        IMOK,
        ITEM,
        LAST,
        LINB,
        LIST,
        MESS,
        OLDM,
        REGI,
        RENO,
        REOK,
        RUOK
    }

    public static MsgType getMsgType(String[] msg){
        switch(msg[0]){
            case "ACKM" : return MsgType.ACKM;
            case "DIFF" : return MsgType.DIFF;
            case "ENDM" : return MsgType.ENDM;
            case "IMOK" : return MsgType.IMOK;
            case "ITEM" : return MsgType.ITEM;
            case "LAST" : return MsgType.LAST;
            case "LINB" : return MsgType.LINB;
            case "LIST" : return MsgType.LIST;
            case "MESS" : return MsgType.MESS;
            case "OLDM" : return MsgType.OLDM;
            case "REGI" : return MsgType.REGI;
            case "RENO" : return MsgType.RENO;
            case "REOK" : return MsgType.REOK;
            case "RUOK" : return MsgType.RUOK;
            default : throw new IllegalArgumentException("Wrong message format");
        }
    }

    public static String getID(String[] msg){
        MsgType type = getMsgType(msg);
        switch(type){
            case OLDM : return msg[2];
            case DIFF : return msg[2];
            case MESS : return msg[1];
            case REGI : return msg[1];
            case ITEM : return msg[1];
            default : throw new IllegalArgumentException("Wrong message format");
        }
    }

    public static String getMsg(String[] msg){
        MsgType type = getMsgType(msg);
        switch(type){
            case MESS : return msg[2];
            case DIFF : return msg[3];
            case OLDM : return msg[3];
            default : throw new IllegalArgumentException("Wrong message format");
        }
    }

    public static String getNumMsg(String[] msg){
        MsgType type = getMsgType(msg);
        switch(type){
            case DIFF : return msg[1];
            case OLDM : return msg[1];
            default : throw new IllegalArgumentException("Wrong message format");
        }
    }

    public static int getNbMsg(String[] msg){
        if(getMsgType(msg) == MsgType.OLDM) return Integer.parseInt(msg[1]);
        throw new IllegalArgumentException("Wrong message format");
    }

    public static String getIp1(String[] msg){
        MsgType type = getMsgType(msg);
        if(type == MsgType.REGI || type == MsgType.ITEM) return msg[2];
        throw new IllegalArgumentException("Wrong message format");
    }

    public static String getIp2(String[] msg){
        MsgType type = getMsgType(msg);
        if(type == MsgType.REGI || type == MsgType.ITEM) return msg[4];
        throw new IllegalArgumentException("Wrong message format");
    }

    public static int getPort1(String[] msg){
        MsgType type = getMsgType(msg);
        if(type == MsgType.REGI || type == MsgType.ITEM) return Integer.parseInt(msg[3]);
        throw new IllegalArgumentException("Wrong message format");
    }

    public static int getPort2(String[] msg){
        MsgType type = getMsgType(msg);
        if(type == MsgType.REGI || type == MsgType.ITEM) return Integer.parseInt(msg[5]);
        throw new IllegalArgumentException("Wrong message format");
    }

    public static int getNumDiff(String[] msg){
        if(getMsgType(msg) == MsgType.LINB) return Integer.parseInt(msg[1]);
        throw new IllegalArgumentException("Wrong message format");
    }

    
}