// stores/counter.js
import { defineStore } from 'pinia'

export interface userInfo {
    address?: "string",
    aud: "string",
    email?: "string",
    iss: "string",
    name: "string",
    phone?: "string",
    picture: "string",
    preferred_username: "string",
    sub: "string"
}

export interface account{
    wner: string;
        name: string;
        createdTime: Date;
        updatedTime: string;
        id: string;
        type: string;
        password: string;
        passwordSalt: string;
        displayName: string;
        firstName: string;
        lastName: string;
        avatar: string;
        permanentAvatar: string;
        email: string;
        emailVerified: boolean;
        phone: string;
        location: string;
        address: any[];
        affiliation: string;
        title: string;
        idCardType: string;
        idCard: string;
        homepage: string;
        bio: string;
        tag: string;
        region: string;
        language: string;
        gender: string;
        birthday: string;
        education: string;
        score: number;
        karma: number;
        ranking: number;
        isDefaultAvatar: boolean;
        isOnline: boolean;
        isAdmin: boolean;
        isGlobalAdmin: boolean;
        isForbidden: boolean;
        isDeleted: boolean;
        signupApplication: string;
        hash: string;
        preHash: string;
        createdIp: string;
        lastSigninTime: string;
        lastSigninIp: string;
        github: string;
        google: string;
        qq: string;
        wechat: string;
        facebook: string;
        dingtalk: string;
        weibo: string;
        gitee: string;
        linkedin: string;
        wecom: string;
        lark: string;
        gitlab: string;
        adfs: string;
        baidu: string;
        alipay: string;
        casdoor: string;
        infoflow: string;
        apple: string;
        azuread: string;
        slack: string;
        steam: string;
        bilibili: string;
        okta: string;
        douyin: string;
        custom: string;
        webauthnCredentials?: any;
        ldap: string;
        properties: any;
        roles?: any;
        permissions?: any;
        lastSigninWrongTime: string;
        signinWrongTimes: number;
        managedAccounts?: any;
}

export interface roleItem{
    isEnabled:boolean,
    name:string,
}

export enum DeviceType {
    CAN ='CAN',
    LIN ='LIN',
    ETH ='ETH',
    LP = 'LP',
}

export enum EncodeType {
    HEX ='HEX',
    ASCII ='ASCII',
    BYTE ='BYTE',
    FLOAT = 'FLOAT',
    DOUBLE = 'DOUBLE',
}


export interface DataItem{
    name:string,
    value:string,
    encode:EncodeType,
    length:number,
}

export interface Frame{
    useWorkerFunc:boolean
    name?:string
    desc?:string
    txFuncName?:string
    rxFuncName?:string
    txData?:DataItem[]
    rxData?:DataItem[]
    options?:any
}

export interface State {
   device: string
   vendor: string
   type: DeviceType
   connectParam:any
   frame:Frame[]
   options:any
}

export const useUdsStore = defineStore('uds', {
    state: (): State => ({
        device:'',
        vendor:'PEAK',
        type:DeviceType.CAN,
        connectParam:{},
        frame:[],
        options:{}
    }),
    actions: {
       
    }
})