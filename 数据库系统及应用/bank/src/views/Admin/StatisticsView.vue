<script setup>
    import Admin from '@/components/Admin.vue'

    // 业务统计:按业务分类(储蓄、贷款)和时间(月、季、年)统计各个支行的业务总金
    //      额和用户数,统计的结果以表格形式展示。
</script>

<script>
    import queryServer from '@/utils/queryServer'

    // create table subbranch
    // (
    //    subbranchName        char(20) not null,
    //    subbranchCity        char(20) not null,
    //    subbranchWealth      float(8,2) not null,
    //    primary key (subbranchName)
    // );

    // create table depositAccount
    // (
    //    accountID            char(30) not null,
    //    subbranchName        char(20) not null,
    //    accountRemain        float(8,2) not null,
    //    accountOpenDate      date not null,
    //    moneyRate            float not null,
    //    currencyType         char(10) not null,
    //    primary key (accountID)
    // );

    // create table checkAccount
    // (
    //    accountID            char(30) not null,
    //    subbranchName        char(20) not null,
    //    accountRemain        float(8,2) not null,
    //    accountOpenDate      date not null,
    //    overdraft            float(8,2) not null,
    //    primary key (accountID)
    // );

    queryServer("insert into subbranch values ('支行A', '北京', '100000')")
    queryServer("insert into subbranch values ('支行B', '上海', '200000')")
    queryServer("insert into subbranch values ('支行C', '广州', '300000')")

    export default {
        data() {
            return {
                subbranchData: [],
                inputTimeType: 0, // 0 表示月, 1 表示季, 2 表示年
                inputServiceType: 0, // 0 表示储蓄, 1 表示贷款
                inputYear: 2020,
                inputMonth: 1,
                inputQuarter: 1
            }
        },
        methods: {
            queryInputData() {
                console.log(this.inputTimeType)
                console.log(this.inputServiceType)
                let rawAccountTable, accountClientTable, accountTable
                if (this.inputServiceType === 0) {
                    rawAccountTable = 'depositAccount'
                    accountClientTable = 'depositClient'
                } else if (this.inputServiceType === 1) {
                    rawAccountTable = 'checkAccount'
                    accountClientTable = 'checkClient'
                } else {
                    return
                }
                console.log(rawAccountTable)
                console.log(accountClientTable)
                if (this.inputTimeType === 2) {
                    accountTable = `select * from ${rawAccountTable} where year(accountOpenDate) = ${this.inputYear}`
                } else if (this.inputTimeType === 0) {
                    accountTable = `select * from ${rawAccountTable} where year(accountOpenDate) = ${this.inputYear} and month(accountOpenDate) = ${this.inputMonth}`
                } else if (this.inputTimeType === 1) {
                    let monthStart = (this.inputQuarter - 1) * 3 + 1
                    let monthEnd = this.inputQuarter * 3
                    accountTable = `select * from ${rawAccountTable} where year(accountOpenDate) = ${this.inputYear} and month(accountOpenDate) between ${monthStart} and ${monthEnd}`
                }

                let command = `select subbranch.subbranchName, temp1.sumUserRemain, temp2.userCount
                    from subbranch left join
                        (select subbranchName, sum(accountRemain) as sumUserRemain from (${accountTable}) as T1 group by subbranchName) as temp1
                    on subbranch.subbranchName = temp1.subbranchName
                    left join
                        (select subbranchName, count(clientID) as userCount from (${accountTable}) as T2 natural join ${accountClientTable} group by subbranchName) as temp2
                    on subbranch.subbranchName = temp2.subbranchName`
                this.subbranchData = queryServer(command)
            }
        }
    }
</script>

<template>
    <Admin highlight="3" />

    <div>
        <button @click="queryInputData" class="queryButton">查询数据</button>
    </div>

    <div class="inputAll">
        <div class="serviceType">
            <label>业务类型:</label>
            <input type="radio" value="0" v-model="inputServiceType" class="radio" @change="inputServiceType = 0" />储蓄
            <input type="radio" value="1" v-model="inputServiceType" class="radio" @change="inputServiceType = 1" />贷款
        </div>
        <div class="timeType">
            <label>时间类型:</label>
            <input type="radio" value="0" v-model="inputTimeType" class="radio" @change="inputTimeType = 0" />月
            <input type="radio" value="1" v-model="inputTimeType" class="radio" @change="inputTimeType = 1" />季
            <input type="radio" value="2" v-model="inputTimeType" class="radio" @change="inputTimeType = 2" />年
        </div>
        <div class="timeInput">
            <label>时间:</label>
            <div class="inputYear"><input type="text" v-model="inputYear" class="inputText" />年</div>
            <div class="inputMonth" v-if="inputTimeType === 1">
                <input type="text" v-model="inputQuarter" class="inputText" />季度
            </div>
            <div class="inputMonth" v-if="inputTimeType === 0">
                <input type="text" v-model="inputMonth" class="inputText" />月
            </div>
        </div>
    </div>

    <div class="subbranchTable">
        <table class="table">
            <thead>
                <tr>
                    <th>支行名称</th>
                    <th>支行用户总余额</th>
                    <th>用户数</th>
                </tr>
            </thead>
            <tbody>
                <tr v-for="(item, index) in subbranchData">
                    <td>{{ item.subbranchName }}</td>
                    <td>{{ item.sumUserRemain }}</td>
                    <td>{{ item.userCount }}</td>
                </tr>
            </tbody>
        </table>
    </div>
</template>

<style src="@/assets/queryTableView.css"></style>

<style>
    .radio {
        width: 30px;
    }
    .inputText {
        width: 50px;
        margin-left: 10px;
    }
    .timeInput {
        display: flex;
        flex-direction: row;
    }
</style>
