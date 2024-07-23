<template>
  <div>
    <el-row :gutter="40">
      <el-col :span="11" v-for="item in flights" :key="item">
        <el-card class="box-card">
          <el-row :gutter="25">
            <el-col :span="11">
              <span style="color: brown;margin-right:15px">{{ item.departure_city_name }} </span>
              <span style="color: coral">{{
                item.depart_time | formatDate()
              }}</span></el-col
            >
            <el-col :span="11">
              <span style="color: coral">{{
                item.arrive_time | formatDate()
              }}</span>
              <span style="color: brown;margin-left:15px">
                {{ item.destination_city_name }}</span
              ></el-col
            >
          </el-row>
          <p>
            <span style="color: yellowgreen; margin-right: 20px"
              >￥{{ item.price }}</span
            >
            <span style="color: orchid; margin-right: 20px"
              >{{ item.mileage }}km</span
            >
            <span style="color: skyblue">用时 {{ item.time_cost }}</span>
          </p>
          <el-button
            style="float: right; padding: 5px; color: red"
            class="el-icon-delete"
            @click="tryDelVisible = true"
            v-if="GLOBAL.hasLogin"
          ></el-button>
          <el-dialog title="确认删除？" :visible.sync="tryDelVisible">
            <div>
              <el-button type="primary" @click="deleteItem(item.id)"
                >确认</el-button
              >
              <el-button @click="tryDelVisible = false">取消</el-button>
            </div>
          </el-dialog>
          <!--
          <el-button
            style="float: right; padding: 5px; color: blue"
            class="el-icon-edit"
            @click="editItem"
            v-if="GLOBAL.hasLogin"
          ></el-button>
          <el-dialog title="编辑航班" :visible.sync="tryEditVisible">
          </el-dialog>
          -->
        </el-card>
      </el-col>
    </el-row>
  </div>
</template>

<script>
export default {
  name: "aFlight",
  data() {
    return {
      tryDelVisible: false,
      tryEditVisible: false,
    };
  },
  props: {
    flights: {
      type: Array,
      required:true
    }
  },
  filters: {
    formatDate: function (value) {
      let dt = new Date(value);
      let year = dt.getFullYear();
      let month = (dt.getMonth() + 1).toString().padStart(2, "0");
      let date = dt.getDate().toString().padStart(2, "0");
      let hour = dt.getHours().toString().padStart(2, "0");
      let minute = dt.getMinutes().toString().padStart(2, "0");
      let second = dt.getSeconds().toString().padStart(2, "0");
      return `${year}-${month}-${date} ${hour}:${minute}:${second}`;
    },
  },
  methods: {
    deleteItem(id) {
      this.$axios({
        method: "delete",
        url: `/transportation/flights/${id}/`,
        headers: {
          "Content-Type": "application/json",
          Authorization: `Bearer ${localStorage.getItem("admin_access")}`,
        },
      }).then(() => {
        this.$emit("flightDeleted");
      });
      this.tryDelVisible = false;
    },
    // editItem() {},
  },
};
</script>