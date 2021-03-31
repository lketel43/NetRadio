import java.nio.ByteBuffer;
import java.util.Arrays;

public class Message{
    // Taille des caracteristiques d'un message en octets
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


    /*Methods to create messages*/

    private static byte[] intToCharBytes(String str, int nbBytes){
        byte[] bytes = new byte[nbBytes];
        Arrays.fill(bytes, (byte) DEFAULT_INT);
        byte[] tmpBytes = str.getBytes();
        int index = nbBytes - tmpBytes.length;
        for(int i = index; i < nbBytes; i++)  bytes[i] = (byte) tmpBytes[i - index];
        return bytes;
    }

    private static byte[] msgToBytes(String msg){
        byte[] bytesMsg = new byte[MSG_MESS_SIZE];
        byte[] tmpBytesMsg = msg.getBytes();
        for(int i = 0; i < MSG_MESS_SIZE; i++){
            if(i < tmpBytesMsg.length) bytesMsg[i] = tmpBytesMsg[i];
            else bytesMsg[i] = (byte) DEFAULT_CHAR;
        }
        return bytesMsg;
    }

    private static String getMsgFromString(String mess){
        String[] fields = mess.split(" ");
        switch(fields[0]){
            case "OLDM" : case "DIFF" :
                int index = MSG_TYPE_SIZE + fields[1].length() + fields[2].length() + 3;
                return mess.substring(index, mess.length());
            case "MESS" : 
                int index2 = MSG_TYPE_SIZE + fields[1].length() + 2;
                return mess.substring(index2, mess.length());
            default : throw new IllegalArgumentException("Wrong message format");
        }
    }

    
    private static byte[] ipToBytes(String ip){
        byte[] bytes = ip.getBytes();
        if(bytes.length < 15){
            byte[] allBytes = new byte[MSG_IP_SIZE];
            Arrays.fill(allBytes, (byte) DEFAULT_INT);
            allBytes[3] = (byte) '.';
            allBytes[7] = (byte) '.';
            allBytes[11] = (byte) '.';
            String[] fields = ip.split("\\.");
            for(int i = 0; i < fields.length; i++){
                int index = 3 - fields[i].length();
                for(int j = index; j < index + fields[i].length(); j++){
                    allBytes[(4 * i) + j] = (byte) fields[i].charAt(j - index);
                }
            }
            return allBytes;
        }
        return bytes;
    }

    private static byte[] typeToBytes(String type){
        return type.getBytes();
    }

    private static byte[] idToBytes(String id){
        byte[] idBytes = new byte[MSG_ID_SIZE];
        Arrays.fill(idBytes, (byte) DEFAULT_CHAR);
        for(int i = 0; i < id.length(); i++) idBytes[i] = (byte) id.charAt(i);
        return idBytes;
    }

    private static byte[] concatByteArray(byte[] ...args){
        int size = 0;
        for(byte[] arr : args) size += arr.length;
        byte[] ret = new byte[size];
        int index = 0;
        for(byte[] arr : args){
            for(int i = 0; i < arr.length; i++) ret[i + index] = arr[i];
            index += arr.length;
        }
        return ret;
    }

    private static byte[] createMsgFromBytes(byte[] ...args){
        if(args.length == 0) return null;
        else return concatByteArray(args);
    }

    public static byte[] createMsg(String msg){
        String[] fields = msg.split(" ");
        switch(fields[0]){
            case "IMOK" : case "RUOK" : case "LIST" :
            case "RENO" : case "REOK" : case "ACKM" :
            case "ENDM" :
                return typeToBytes(fields[0]);
            case "REGI" : case "ITEM":
                byte[] type = typeToBytes(fields[0]);
                byte[] id = idToBytes(fields[1]);
                byte[] ip1 = ipToBytes(fields[2]);
                byte[] port1 = intToCharBytes(fields[3], MSG_PORT_SIZE);
                byte[] ip2 = ipToBytes(fields[4]);
                byte[] port2 = intToCharBytes(fields[5], MSG_PORT_SIZE);
                byte[][] bytes = {type, id, ip1, port1, ip2, port2};
                return createMsgFromBytes(bytes);
            case "OLDM" : case "DIFF" :
                byte[] type2 = typeToBytes(fields[0]);
                byte[] numMsg = intToCharBytes(fields[1], MSG_NUM_MESS_SIZE);
                byte[] id2 = idToBytes(fields[2]);
                String strMsg = getMsgFromString(msg);
                byte[] mess = msgToBytes(strMsg);
                byte[][] bytes2 = {type2, numMsg, id2, mess};
                return createMsgFromBytes(bytes2); 
            case "LAST" :
                byte[] type3 = typeToBytes(fields[0]);
                byte[] nbMsg = intToCharBytes(fields[1], MSG_NB_MESS);
                byte[][] bytes3 = {type3, nbMsg};
                return createMsgFromBytes(bytes3);
            case "MESS" :
                byte[] type4 = typeToBytes(fields[0]);
                byte[] id3 = idToBytes(fields[1]);
                String strMsg2 = getMsgFromString(msg);
                byte[] mess2 = msgToBytes(strMsg2);
                byte[][] bytes4 = {type4, id3, mess2};
                return createMsgFromBytes(bytes4);
            case "LINB" :
                byte[] type5 = typeToBytes(fields[0]);
                byte[] numDiff = intToCharBytes(fields[1], MSG_NUM_DIFF_SIZE);
                byte[][] bytes5 = {type5, numDiff};
                return createMsgFromBytes(bytes5);
            default : throw new IllegalArgumentException("Wrong message format");

        }
    }
}